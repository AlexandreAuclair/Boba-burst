#include "bb_gameScene.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <cstring> //pour memcpy

// Include de debug pour conna�tre r�pertoire travail courant
#include <filesystem>
#include <iostream>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <cmath>

// Definition constructeur surcharge
GameScene::GameScene(
    VulkanContext &context)
    : m_context(context)
{

    CreateVertexBuffer();
    CreateIndexBuffer();

    // Creation des attributs de la lumiere
    // =======================================
    m_lightData.direction = glm::vec4(glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)), 0.0f);

    // Lumiere couleur blanche
    m_lightData.color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);

    // Init avec vecteur bidon car elle sera �cras�e � chaque frame dans Draw().
    m_lightData.cameraPosition = glm::vec4(20.0f, 20.0f, 20.0f, 1.0f);
    // =======================================

    CreateLightResources();

    for (auto &[name, tex] : GameManager::get().textures)
    {
        SendTextureToGPU(*tex);
    }

    CreatePipelineLayout();
}

void GameScene::Update(double dt)
{
    Scene::Update(dt);

    // UpdateLightBuffer(cam.GetPosition());
}

void GameScene::Draw(const DrawInfo &drawInfo)
{
    if (!m_skyboxPipeline)
    {
        CreatePipeline(drawInfo.renderPass, m_skyboxPipeline, "./shaders_out/global.vert.spv", "./shaders_out/skybox.frag.spv", VK_FALSE);
    }
    if (!m_pipeline)
    {
        CreatePipeline(drawInfo.renderPass, m_pipeline, "./shaders_out/global.vert.spv", "./shaders_out/global.frag.spv", VK_TRUE);
    }

    auto &cmd = drawInfo.commandBuffer;
    auto &gm = GameManager::get();

    // Matrices caméra (View et Projection)
    auto surfaceExtents = m_context.GetSurfaceExtents();
    float aspectRatio = static_cast<float>(surfaceExtents.width) / static_cast<float>(surfaceExtents.height);
    glm::mat4 projMat;
    glm::mat4 viewMat;

    if (!gm.players.empty() && gm.players[0])
    {
        gm.players[0]->cam.frustum = gm.players[0]->cam.frustum.ExtractFrustum(projMat * viewMat);
        const Camera &cam = gm.players[0]->cam;
        projMat = cam.GetProjectionMatrix(aspectRatio);
        viewMat = cam.GetViewMatrix();
    }
    else
    {
        projMat = glm::perspective(glm::pi<float>() * 0.25f, aspectRatio, 0.1f, 1000.f);
        viewMat = glm::lookAt(glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::mat4 viewProjMat = projMat * viewMat;

    // Fonction d'aide pour dessiner un Actor
    auto drawActor = [&](const Actor &actor)
    {

        if (!IsSphereVisible(actor.mesh->bs.GetWorldBoundingSphere(actor.mesh->bs, viewProjMat), gm.players[0]->cam.frustum))
        {
            Logger::Log("actor has been culled!");
            return;
        }

        if (!actor.mesh || actor.mesh->indexes.empty())
            return;

        if (!actor.mesh || actor.mesh->indexes.empty())
            return;

        // Vérifier que la texture ET son descriptorSet existent avant de dessiner !
        if (!actor.texture || !actor.texture->descriptorSet)
        {
            // Option A : Ne pas dessiner l'objet et afficher un log pour identifier lequel pose problème
            Logger::Warn("Actor ignore au rendu : texture ou descriptorSet manquant.");

            // Option B (recommandée) : Utiliser une texture par défaut
            cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, *m_pipelineLayout, 0, {GameManager::get().textures.begin()->second->descriptorSet}, {});
        }
        else
        {
            cmd.bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics,
                *m_pipelineLayout,
                0,
                {actor.texture->descriptorSet},
                {});
        }

        // Lier les buffers de géométrie du Mesh
        cmd.bindVertexBuffers(0, {*actor.mesh->vertexBuffer}, {0});
        cmd.bindIndexBuffer(*actor.mesh->indexBuffer, 0, vk::IndexType::eUint32);

        // Calculer la matrice monde de cet acteur
        glm::mat4 worldMat = glm::mat4(1.0f);
        worldMat = glm::translate(worldMat, actor.GetTransform().position);
        worldMat = glm::rotate(worldMat, glm::radians(actor.GetTransform().rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        worldMat = glm::rotate(worldMat, glm::radians(actor.GetTransform().rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        worldMat = glm::rotate(worldMat, glm::radians(actor.GetTransform().rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        worldMat = glm::scale(worldMat, actor.GetTransform().scale);

        Matrices pushMatrices;
        pushMatrices.worldMatrix = worldMat;
        pushMatrices.worldViewProjMatrix = viewProjMat * worldMat;

        cmd.pushConstants(
            *m_pipelineLayout,
            vk::ShaderStageFlagBits::eVertex,
            0,
            sizeof(Matrices),
            &pushMatrices);

        cmd.drawIndexed(
            static_cast<uint32_t>(actor.mesh->indexes.size()),
            1, 0, 0, 0);
    };

    if (!gm.characters.empty())
    {

        cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_skyboxPipeline);
        for (const auto &character : gm.characters)
        {
            if (character)
                drawActor(*character);
        }
    }

    cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_pipeline);

    // Dessiner tous les Tracks et Players du GameManager
    for (const auto &track : gm.tracks)
    {
        if (track)
            drawActor(*track);
    }

    for (const auto &player : gm.players)
    {
        if (player)
            drawActor(*player);
    }
}

void GameScene::CreateAndUploadBuffer(
    const void *srcData,
    vk::DeviceSize size,
    vk::BufferUsageFlags usage,
    vk::UniqueBuffer &outBuffer,
    vk::UniqueDeviceMemory &outMemory)
{
    auto bufferCreateInfo = vk::BufferCreateInfo{};
    bufferCreateInfo.setSize(size);
    bufferCreateInfo.setUsage(usage);

    outBuffer = m_context.GetDevice().createBufferUnique(bufferCreateInfo);

    auto memoryReqs = m_context.GetDevice().getBufferMemoryRequirements(*outBuffer);

    uint32_t memoryTypeIndex = m_context.GetMemoryTypeIndex(
        memoryReqs.memoryTypeBits,
        vk::MemoryPropertyFlagBits::eHostVisible |
            vk::MemoryPropertyFlagBits::eDeviceLocal);
    assert(memoryTypeIndex != static_cast<uint32_t>(-1));

    auto memoryAllocateInfo = vk::MemoryAllocateInfo{};
    memoryAllocateInfo.setAllocationSize(memoryReqs.size);
    memoryAllocateInfo.setMemoryTypeIndex(memoryTypeIndex);

    outMemory = m_context.GetDevice().allocateMemoryUnique(memoryAllocateInfo);

    m_context.GetDevice().bindBufferMemory(*outBuffer, *outMemory, 0);

    void *bufferPtr = m_context.GetDevice().mapMemory(*outMemory, 0, size);
    std::memcpy(bufferPtr, srcData, static_cast<size_t>(size));
    m_context.GetDevice().unmapMemory(*outMemory);
}

void GameScene::CreateVertexBuffer()
{
    auto &gm = GameManager::get();

    for (auto &[modelName, mesh] : gm.models)
    {
        if (!mesh || mesh->sommets.empty())
            continue;

        vk::DeviceSize taille = mesh->sommets.size() * sizeof(Sommet);

        CreateAndUploadBuffer(
            mesh->sommets.data(),
            taille,
            vk::BufferUsageFlagBits::eVertexBuffer,
            mesh->vertexBuffer,
            mesh->vertexBufferMemory);
    }
}

void GameScene::CreateIndexBuffer()
{
    auto &gm = GameManager::get();

    for (auto &[modelName, mesh] : gm.models)
    {
        if (!mesh || mesh->indexes.empty())
            continue;

        vk::DeviceSize taille = mesh->indexes.size() * sizeof(uint32_t);

        CreateAndUploadBuffer(
            mesh->indexes.data(),
            taille,
            vk::BufferUsageFlagBits::eIndexBuffer,
            mesh->indexBuffer,
            mesh->indexBufferMemory);
    }
}
// Cr�� mes ressources pour les lights, notamment les buffer
void GameScene::CreateLightResources()
{
    // ---------- 1. Buffer uniforme ----------
    vk::DeviceSize taille = sizeof(LightData);
    auto bufferCreateInfo = vk::BufferCreateInfo{};
    bufferCreateInfo.setSize(taille);
    bufferCreateInfo.setUsage(vk::BufferUsageFlagBits::eUniformBuffer);

    m_lightBuffer = m_context.GetDevice().createBufferUnique(bufferCreateInfo);

    auto memoryReqs = m_context.GetDevice().getBufferMemoryRequirements(*m_lightBuffer);

    uint32_t memoryTypeIndex = m_context.GetMemoryTypeIndex(
        memoryReqs.memoryTypeBits,
        vk::MemoryPropertyFlagBits::eHostVisible |
            vk::MemoryPropertyFlagBits::eHostCoherent);

    assert(memoryTypeIndex != static_cast<uint32_t>(-1));

    auto memoryAllocateInfo = vk::MemoryAllocateInfo{};
    memoryAllocateInfo.setAllocationSize(memoryReqs.size);
    memoryAllocateInfo.setMemoryTypeIndex(memoryTypeIndex);

    m_lightBufferMemory = m_context.GetDevice().allocateMemoryUnique(memoryAllocateInfo);
    m_context.GetDevice().bindBufferMemory(*m_lightBuffer, *m_lightBufferMemory, 0);

    m_lightBufferMapped = m_context.GetDevice().mapMemory(*m_lightBufferMemory, 0, taille);
    std::memcpy(m_lightBufferMapped, &m_lightData, sizeof(LightData));

    // ---------- 2. Descriptor set layout : binding 0 = lumière, binding 1 = texture ----------
    std::array<vk::DescriptorSetLayoutBinding, 2> bindings;

    bindings[0].setBinding(0);
    bindings[0].setDescriptorType(vk::DescriptorType::eUniformBuffer);
    bindings[0].setDescriptorCount(1);
    bindings[0].setStageFlags(vk::ShaderStageFlagBits::eFragment);

    bindings[1].setBinding(1);
    bindings[1].setDescriptorType(vk::DescriptorType::eCombinedImageSampler);
    bindings[1].setDescriptorCount(1);
    bindings[1].setStageFlags(vk::ShaderStageFlagBits::eFragment);

    auto layoutCreateInfo = vk::DescriptorSetLayoutCreateInfo{};
    layoutCreateInfo.setBindings(bindings);

    m_lightDescriptorSetLayout =
        m_context.GetDevice().createDescriptorSetLayoutUnique(layoutCreateInfo);

    // ---------- 3. Descriptor pool dimensionné pour toutes les textures ----------
    uint32_t maxTextures = std::max<uint32_t>(1, static_cast<uint32_t>(GameManager::get().textures.size()));

    std::array<vk::DescriptorPoolSize, 2> poolSizes;
    poolSizes[0].setType(vk::DescriptorType::eUniformBuffer);
    poolSizes[0].setDescriptorCount(maxTextures);
    poolSizes[1].setType(vk::DescriptorType::eCombinedImageSampler);
    poolSizes[1].setDescriptorCount(maxTextures);

    auto poolCreateInfo = vk::DescriptorPoolCreateInfo{};
    poolCreateInfo.setMaxSets(maxTextures);
    poolCreateInfo.setPoolSizes(poolSizes);

    m_lightDescriptorPool =
        m_context.GetDevice().createDescriptorPoolUnique(poolCreateInfo);
}

// Update le light buffer avec la position de la cam�ra
void GameScene::UpdateLightBuffer(const glm::vec3 &cameraPosition)
{
    m_lightData.cameraPosition = glm::vec4(cameraPosition, 1.0f);
    std::memcpy(m_lightBufferMapped, &m_lightData, sizeof(LightData));
}

void GameScene::CreatePipelineLayout()
{
    auto pushConstantRange = vk::PushConstantRange{};
    pushConstantRange.setSize(sizeof(Matrices));
    pushConstantRange.setStageFlags(vk::ShaderStageFlagBits::eVertex);

    auto pipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo{};
    pipelineLayoutCreateInfo.setSetLayouts({*m_lightDescriptorSetLayout});
    pipelineLayoutCreateInfo.setPushConstantRanges({pushConstantRange});

    m_pipelineLayout =
        m_context.GetDevice().createPipelineLayoutUnique(pipelineLayoutCreateInfo);
}

void GameScene::SendTextureToGPU(Texture &tex)
{
    vk::DeviceSize imageSize = static_cast<vk::DeviceSize>(tex.width) * tex.height * 4;
    auto device = m_context.GetDevice();

    // ---------- 2. Staging buffer ----------
    auto stagingInfo = vk::BufferCreateInfo{};
    stagingInfo.setSize(imageSize);
    stagingInfo.setUsage(vk::BufferUsageFlagBits::eTransferSrc);
    auto stagingBuffer = device.createBufferUnique(stagingInfo);

    auto stagingReqs = device.getBufferMemoryRequirements(*stagingBuffer);
    auto stagingAlloc = vk::MemoryAllocateInfo{};
    stagingAlloc.setAllocationSize(stagingReqs.size);
    stagingAlloc.setMemoryTypeIndex(m_context.GetMemoryTypeIndex(
        stagingReqs.memoryTypeBits,
        vk::MemoryPropertyFlagBits::eHostVisible |
            vk::MemoryPropertyFlagBits::eHostCoherent));
    auto stagingMemory = device.allocateMemoryUnique(stagingAlloc);
    device.bindBufferMemory(*stagingBuffer, *stagingMemory, 0);

    void *data = device.mapMemory(*stagingMemory, 0, imageSize);
    std::memcpy(data, tex.pixels, static_cast<size_t>(imageSize));
    device.unmapMemory(*stagingMemory);

    // ---------- 3. Image GPU ----------
    auto imageInfo = vk::ImageCreateInfo{};
    imageInfo.setImageType(vk::ImageType::e2D);
    imageInfo.setFormat(vk::Format::eR8G8B8A8Srgb);
    imageInfo.setExtent({static_cast<uint32_t>(tex.width), static_cast<uint32_t>(tex.height), 1});
    imageInfo.setMipLevels(1);
    imageInfo.setArrayLayers(1);
    imageInfo.setSamples(vk::SampleCountFlagBits::e1);
    imageInfo.setTiling(vk::ImageTiling::eOptimal);
    imageInfo.setUsage(vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled);
    imageInfo.setInitialLayout(vk::ImageLayout::eUndefined);
    tex.image = device.createImageUnique(imageInfo);

    auto imageReqs = device.getImageMemoryRequirements(*(tex.image));
    auto imageAlloc = vk::MemoryAllocateInfo{};
    imageAlloc.setAllocationSize(imageReqs.size);
    imageAlloc.setMemoryTypeIndex(m_context.GetMemoryTypeIndex(
        imageReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal));
    tex.memory = device.allocateMemoryUnique(imageAlloc);
    device.bindImageMemory(*(tex.image), *(tex.memory), 0);

    // ---------- 4. Copie staging -> image ----------
    auto cmdAlloc = vk::CommandBufferAllocateInfo{};
    cmdAlloc.setCommandPool(m_context.GetCommandPool());
    cmdAlloc.setLevel(vk::CommandBufferLevel::ePrimary);
    cmdAlloc.setCommandBufferCount(1);
    auto cmdBuffers = device.allocateCommandBuffersUnique(cmdAlloc);
    auto &cmd = cmdBuffers[0];

    auto beginInfo = vk::CommandBufferBeginInfo{};
    beginInfo.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
    cmd->begin(beginInfo);

    auto range = vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1};

    auto toTransfer = vk::ImageMemoryBarrier{};
    toTransfer.setOldLayout(vk::ImageLayout::eUndefined);
    toTransfer.setNewLayout(vk::ImageLayout::eTransferDstOptimal);
    toTransfer.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
    toTransfer.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
    toTransfer.setImage(*tex.image);
    toTransfer.setSubresourceRange(range);
    toTransfer.setSrcAccessMask({});
    toTransfer.setDstAccessMask(vk::AccessFlagBits::eTransferWrite);
    cmd->pipelineBarrier(
        vk::PipelineStageFlagBits::eTopOfPipe,
        vk::PipelineStageFlagBits::eTransfer,
        {}, {}, {}, toTransfer);

    auto region = vk::BufferImageCopy{};
    region.setImageSubresource({vk::ImageAspectFlagBits::eColor, 0, 0, 1});
    region.setImageExtent({static_cast<uint32_t>(tex.width), static_cast<uint32_t>(tex.height), 1});
    cmd->copyBufferToImage(
        *stagingBuffer, *(tex.image),
        vk::ImageLayout::eTransferDstOptimal, region);

    auto toShader = toTransfer;
    toShader.setOldLayout(vk::ImageLayout::eTransferDstOptimal);
    toShader.setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal);
    toShader.setSrcAccessMask(vk::AccessFlagBits::eTransferWrite);
    toShader.setDstAccessMask(vk::AccessFlagBits::eShaderRead);
    cmd->pipelineBarrier(
        vk::PipelineStageFlagBits::eTransfer,
        vk::PipelineStageFlagBits::eFragmentShader,
        {}, {}, {}, toShader);

    cmd->end();

    vk::CommandBuffer cmdHandle = *cmd;
    auto submitInfo = vk::SubmitInfo{};
    submitInfo.setCommandBuffers(cmdHandle);
    m_context.GetQueue().submit(submitInfo);
    m_context.GetQueue().waitIdle();

    // ---------- 5. Image view ----------
    auto viewInfo = vk::ImageViewCreateInfo{};
    viewInfo.setImage(*(tex.image));
    viewInfo.setViewType(vk::ImageViewType::e2D);
    viewInfo.setFormat(vk::Format::eR8G8B8A8Srgb);
    viewInfo.setSubresourceRange(range);
    tex.view = device.createImageViewUnique(viewInfo);

    // ---------- 6. Sampler ----------
    auto samplerInfo = vk::SamplerCreateInfo{};
    samplerInfo.setMagFilter(vk::Filter::eLinear);
    samplerInfo.setMinFilter(vk::Filter::eLinear);
    samplerInfo.setMipmapMode(vk::SamplerMipmapMode::eLinear);
    samplerInfo.setAddressModeU(vk::SamplerAddressMode::eRepeat);
    samplerInfo.setAddressModeV(vk::SamplerAddressMode::eRepeat);
    samplerInfo.setAddressModeW(vk::SamplerAddressMode::eRepeat);
    tex.sampler = device.createSamplerUnique(samplerInfo);

    // ---------- 7. Descriptor Set pour cette texture ----------
    auto descAllocInfo = vk::DescriptorSetAllocateInfo{};
    descAllocInfo.setDescriptorPool(*m_lightDescriptorPool);
    descAllocInfo.setSetLayouts(*m_lightDescriptorSetLayout);
    tex.descriptorSet = device.allocateDescriptorSets(descAllocInfo)[0];

    auto lightBufferInfo = vk::DescriptorBufferInfo{};
    lightBufferInfo.setBuffer(*m_lightBuffer);
    lightBufferInfo.setOffset(0);
    lightBufferInfo.setRange(sizeof(LightData));

    auto writeLight = vk::WriteDescriptorSet{};
    writeLight.setDstSet(tex.descriptorSet);
    writeLight.setDstBinding(0);
    writeLight.setDstArrayElement(0);
    writeLight.setDescriptorType(vk::DescriptorType::eUniformBuffer);
    writeLight.setBufferInfo(lightBufferInfo);

    auto textureInfo = vk::DescriptorImageInfo{};
    textureInfo.setSampler(*tex.sampler);
    textureInfo.setImageView(*tex.view);
    textureInfo.setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal);

    auto writeTexture = vk::WriteDescriptorSet{};
    writeTexture.setDstSet(tex.descriptorSet);
    writeTexture.setDstBinding(1);
    writeTexture.setDstArrayElement(0);
    writeTexture.setDescriptorType(vk::DescriptorType::eCombinedImageSampler);
    writeTexture.setImageInfo(textureInfo);

    std::array<vk::WriteDescriptorSet, 2> writes = {writeLight, writeTexture};
    device.updateDescriptorSets(writes, {});
}

namespace
{
    struct VERTEX
    {
        glm::vec3 position;
        uint32_t color;
    };

    enum VERTEX_ATTRIBUTES
    {
        POSITION,
        NORMAL,
        UV_COORDS,
    };

    constexpr int g_vertexCount = 8;
    constexpr int g_indexCount = 36;
}

void GameScene::CreatePipeline(vk::RenderPass renderPass, vk::UniquePipeline &pipeline, const std::string &vertPath, const std::string &fragPath, bool depthWrite)
{
    auto vsModule = m_context.CreateShaderModuleFromFile(vertPath.c_str());
    auto psModule = m_context.CreateShaderModuleFromFile(fragPath.c_str());

    std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages;
    shaderStages[0].setModule(*vsModule);
    shaderStages[0].setStage(vk::ShaderStageFlagBits::eVertex);
    shaderStages[0].setPName("main");

    shaderStages[1].setModule(*psModule);
    shaderStages[1].setStage(vk::ShaderStageFlagBits::eFragment);
    shaderStages[1].setPName("main");

    std::vector<vk::VertexInputAttributeDescription> vertexAttributes;

    // Position : x, y, z
    {
        auto attribute = vk::VertexInputAttributeDescription{};
        attribute.setBinding(0);
        attribute.setLocation(POSITION);
        attribute.setFormat(vk::Format::eR32G32B32Sfloat); // 3 float : x, y, z
        attribute.setOffset(offsetof(Sommet, position));   // On commence � regarder � partir de x pour ce binding

        vertexAttributes.push_back(attribute);
    }

    // Normale : nx, ny, nz
    {
        auto attribute = vk::VertexInputAttributeDescription{};
        attribute.setBinding(0);
        attribute.setLocation(NORMAL);
        attribute.setFormat(vk::Format::eR32G32B32Sfloat); // 3 float : nx, ny, nz
        attribute.setOffset(offsetof(Sommet, normal));     // On commence � regarder � partir de nx pour ce binding

        vertexAttributes.push_back(attribute);
    }

    // Coordonn�es de texture : U, V
    {
        auto attribute = vk::VertexInputAttributeDescription{};
        attribute.setBinding(0);
        attribute.setLocation(UV_COORDS);
        attribute.setFormat(vk::Format::eR32G32Sfloat); // 2 float : u, v
        attribute.setOffset(offsetof(Sommet, UVcoord)); // On commence � regarder � partir de u pour ce binding

        vertexAttributes.push_back(attribute);
    }

    auto vertexBinding = vk::VertexInputBindingDescription{};
    vertexBinding.setInputRate(vk::VertexInputRate::eVertex);
    vertexBinding.setStride(sizeof(Sommet));

    auto vertexInputState = vk::PipelineVertexInputStateCreateInfo{};
    vertexInputState.setVertexAttributeDescriptions(vertexAttributes);
    vertexInputState.setVertexBindingDescriptions(vertexBinding);

    auto inputAssemblyState = vk::PipelineInputAssemblyStateCreateInfo{};
    inputAssemblyState.setTopology(vk::PrimitiveTopology::eTriangleList);

    auto rasterizationState = vk::PipelineRasterizationStateCreateInfo{};
    rasterizationState.setLineWidth(1.0f);

    // ACTIVER/DESACTIVER Backface culling
    rasterizationState.setCullMode(vk::CullModeFlagBits::eBack);

    auto viewportState = vk::PipelineViewportStateCreateInfo{};
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    auto depthStencilState = vk::PipelineDepthStencilStateCreateInfo{};
    depthStencilState.setDepthWriteEnable(depthWrite);
    depthStencilState.setDepthTestEnable(depthWrite);
    depthStencilState.setDepthCompareOp(vk::CompareOp::eLessOrEqual);

    auto colorAttachmentBlendState = vk::PipelineColorBlendAttachmentState{};
    colorAttachmentBlendState.colorWriteMask =
        vk::ColorComponentFlagBits::eR |
        vk::ColorComponentFlagBits::eG |
        vk::ColorComponentFlagBits::eB |
        vk::ColorComponentFlagBits::eA;

    std::array<vk::PipelineColorBlendAttachmentState, 1> colorAttachmentBlendStates = {colorAttachmentBlendState};
    auto colorBlendState = vk::PipelineColorBlendStateCreateInfo{};
    colorBlendState.setAttachments(colorAttachmentBlendStates);

    std::array<vk::DynamicState, 2> dynamicStates = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};
    auto dynamicState = vk::PipelineDynamicStateCreateInfo{};
    dynamicState.setDynamicStates(dynamicStates);

    auto multisampleState = vk::PipelineMultisampleStateCreateInfo{};
    multisampleState.rasterizationSamples = vk::SampleCountFlagBits::e1;

    vk::GraphicsPipelineCreateInfo createInfo = {};
    createInfo.setStages(shaderStages);
    createInfo.setRenderPass(renderPass);
    createInfo.setLayout(*m_pipelineLayout);
    createInfo.setPVertexInputState(&vertexInputState);
    createInfo.setPInputAssemblyState(&inputAssemblyState);
    createInfo.setPRasterizationState(&rasterizationState);
    createInfo.setPDepthStencilState(&depthStencilState);
    createInfo.setPViewportState(&viewportState);
    createInfo.setPDynamicState(&dynamicState);
    createInfo.setPColorBlendState(&colorBlendState);
    createInfo.setPMultisampleState(&multisampleState);

    auto result = m_context.GetDevice().createGraphicsPipelineUnique({}, createInfo);
    pipeline = std::move(result.value);
}

bool GameScene::IsSphereVisible(const BoundingSphere& sphere, const Frustum& frustum)
{
    for (const PlaneCulling& plane : frustum.planes)
    {
        float distance =
            glm::dot(plane.normal, sphere.center) + plane.distance;

        if (distance < -sphere.radius)
        {
            return false;
        }
    }

    return true;
}
// -- UI_RENDERER_CPP -- => S'occupe de la création du pipeline 2D --- //

#include "UIRenderer.h"
#include <iostream>

//#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"


// Definition constructeur
UIRenderer::UIRenderer(
	VulkanContext& context,
	vk::RenderPass renderpass) : m_context{ context } {
	
	// 1. Charger la texture en mémoire
	m_pauseTexture = CreateTexture("assets/images/menu_pause_version_2.png");

	// 2. Declarations des ressources necessaires en memoire pour le shader (disposition du descriptor set avant le descriptor set en lui-même)
	CreateUIDescriptorSetLayout();

	// 3. Creer graphique pipeline 2D
	CreateUIPipeline(renderpass);

	// 4. Creer et configurer le descriptor set
	CreateUIDescriptorSet();
}

// Retourner les demi-dimensions de la texture en NDC
UIPushConstants UIRenderer::GetPauseMenuSize(uint32_t framebufferWidth,
	uint32_t framebufferHeight) const {

	return CalculatePauseMenuSize(m_pauseTexture.width,
		m_pauseTexture.height,
		framebufferWidth,
		framebufferHeight);
}

void UIRenderer::Draw(vk::CommandBuffer commandBuffer, vk::Extent2D extent) {
    
    // ETAPE 1 : Activer le pipeline UI
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_ui_pipeline);
    

	// ===================================================
	// ETAPE 1.5 - CONSERVER LES PROPORTIONS DU PNG
	// ===================================================
	
	// Données destinées au Vertex Shader.
	const UIPushConstants pushConstants = GetPauseMenuSize(extent.width, extent.height);

	// Sécurité : pas de dessin si le framebuffer est invalide.
	if (pushConstants.halfWidth <= 0.0f ||
		pushConstants.halfHeight <= 0.0f)
	{
		return;
	}

	// Envoi des Push Constants au Vertex Shader.
	commandBuffer.pushConstants(
		*m_ui_pipelineLayout,
		vk::ShaderStageFlagBits::eVertex,
		0,
		sizeof(UIPushConstants),
		&pushConstants
	);

	// ETAPE 2. Binder le Descriptor Set avec le layout du pipeline UI
	commandBuffer.bindDescriptorSets(
		vk::PipelineBindPoint::eGraphics,
		*m_ui_pipelineLayout,
		0,
		m_uiDescriptorSet,
		{}
	);

    // ETAPE 3. Configurer le viewport (zone où on veut dessiner)
    vk::Viewport viewport{};

    viewport.setX(0.0f);
    viewport.setY(0.0f);
    viewport.setWidth(static_cast<float>(extent.width));
    viewport.setHeight(static_cast<float>(extent.height));
    viewport.setMinDepth(0.0f);
    viewport.setMaxDepth(1.0f);

    commandBuffer.setViewport(0, { viewport });

    // ETAPE 4. Configurer le scissor (zone où on peut dessiner)
    vk::Rect2D scissor{};

    scissor.setOffset({ 0, 0 });
    scissor.setExtent(extent);

    commandBuffer.setScissor(0, { scissor });


    // ETAPE 5 : Enregistre commande dessin 2 triangles
    commandBuffer.draw(
        6,  // Nombre de sommets : 2 triangles
        1,  // Une seule instance
        0,  // Premier sommet
        0   // Première instance
    );
}


// Creation pipeline 2D pour UI au-dessus de la 3D
void UIRenderer::CreateUIPipeline(vk::RenderPass renderPass) {

    // Déclaration des shaders
    auto vsModule = m_context.CreateShaderModuleFromFile("./shaders_out/ui.vert.spv");
    auto psModule = m_context.CreateShaderModuleFromFile("./shaders_out/ui.frag.spv");

    std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages;
    shaderStages[0].setModule(*vsModule);
    shaderStages[0].setStage(vk::ShaderStageFlagBits::eVertex);
    shaderStages[0].setPName("main");

    shaderStages[1].setModule(*psModule);
    shaderStages[1].setStage(vk::ShaderStageFlagBits::eFragment);
    shaderStages[1].setPName("main");


    // Pas de vertex buffer car le vertex shader cree les sommets direct
    vk::PipelineVertexInputStateCreateInfo vertexInputState{};

    auto inputAssemblyState = vk::PipelineInputAssemblyStateCreateInfo{};
    inputAssemblyState.setTopology(vk::PrimitiveTopology::eTriangleList);

    // Désactiver test de profondeur pour 2D
    auto depthStencilState = vk::PipelineDepthStencilStateCreateInfo{};
    depthStencilState.setDepthWriteEnable(VK_FALSE);
    depthStencilState.setDepthTestEnable(VK_FALSE);


    // Gérer la rasterization
    auto rasterizationState = vk::PipelineRasterizationStateCreateInfo{};
    rasterizationState.setLineWidth(1.0f);

    // DESACTIVER le culling car ça sert à rien
    rasterizationState.setCullMode(vk::CullModeFlagBits::eNone);

    // Remplir les triangles
    rasterizationState.setPolygonMode(vk::PolygonMode::eFill);

    // Viewport et scissor
    auto viewportState = vk::PipelineViewportStateCreateInfo{};
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

	// Descriptor Set Layout déjà créé dans le constructeur avec CreateUIDescriptorSetLayout();
	vk::DescriptorSetLayout setLayout = *m_uiDescriptorSetLayout;

	// Création du Pipeline Layout
	vk::PipelineLayoutCreateInfo layoutInfo{};
	layoutInfo.setSetLayouts(setLayout);


	// Ajout du push constant pour avoir bon dimensionnement UI
	//////////////
	vk::PushConstantRange pushConstantRange{};

	// Vertex Shader aura accès aux infos
	pushConstantRange.setStageFlags(
		vk::ShaderStageFlagBits::eVertex
	);

	pushConstantRange.setOffset(0);

	// Taille que l'on pousse
	pushConstantRange.setSize(
		sizeof(UIPushConstants)
	);

	// Ajouter les infos du push constant au layout
	layoutInfo.setPushConstantRanges(pushConstantRange);
	/////////////////////

	m_ui_pipelineLayout =
		m_context.GetDevice().createPipelineLayoutUnique(layoutInfo);

    // Color attachement
    auto colorAttachmentBlendState = vk::PipelineColorBlendAttachmentState{};

	// Pas encore de transparence pour le prototype
	colorAttachmentBlendState.setBlendEnable(VK_FALSE);

    colorAttachmentBlendState.colorWriteMask =
        vk::ColorComponentFlagBits::eR |
        vk::ColorComponentFlagBits::eG |
        vk::ColorComponentFlagBits::eB |
        vk::ColorComponentFlagBits::eA;

    std::array<vk::PipelineColorBlendAttachmentState, 1> colorAttachmentBlendStates = { colorAttachmentBlendState };
   
	auto colorBlendState = vk::PipelineColorBlendStateCreateInfo{};
    colorBlendState.setAttachments(colorAttachmentBlendStates);


    std::array<vk::DynamicState, 2> dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
    auto dynamicState = vk::PipelineDynamicStateCreateInfo{};
    dynamicState.setDynamicStates(dynamicStates);

    auto multisampleState = vk::PipelineMultisampleStateCreateInfo{};
    multisampleState.rasterizationSamples = vk::SampleCountFlagBits::e1;

    vk::GraphicsPipelineCreateInfo createInfo = {};
    createInfo.setStages(shaderStages);
    createInfo.setRenderPass(renderPass);
    createInfo.setLayout(*m_ui_pipelineLayout);
    createInfo.setSubpass(0);
    createInfo.setPVertexInputState(&vertexInputState);
    createInfo.setPInputAssemblyState(&inputAssemblyState);
    createInfo.setPRasterizationState(&rasterizationState);
    createInfo.setPDepthStencilState(&depthStencilState);
    createInfo.setPViewportState(&viewportState);
    createInfo.setPDynamicState(&dynamicState);
    createInfo.setPColorBlendState(&colorBlendState);
    createInfo.setPMultisampleState(&multisampleState);

    auto result = m_context.GetDevice().createGraphicsPipelineUnique({}, createInfo);
    m_ui_pipeline = std::move(result.value);
}


// Creation de la disposition du descriptor set
// Descriptor set 0 et binding 0 <= texture du menu
void UIRenderer::CreateUIDescriptorSetLayout() {
	auto device = m_context.GetDevice();

	// Une seule ressource : notre texture PNG
	vk::DescriptorSetLayoutBinding binding{};

	// Ressource est au binding 0 dans le fragment shader
	binding.setBinding(0);
	binding.setDescriptorType(
		vk::DescriptorType::eCombinedImageSampler
	);

	// DescriptorCount = 1 => 1 seule texture declaree
	binding.setDescriptorCount(1);
	binding.setStageFlags(
		vk::ShaderStageFlagBits::eFragment
	);

	auto layoutInfo = vk::DescriptorSetLayoutCreateInfo{};
	layoutInfo.setBindings(binding);

	// On met toutes les infos sur le descriptor set dans ce parametre
	m_uiDescriptorSetLayout =
		device.createDescriptorSetLayoutUnique(layoutInfo);
}


// Creer le descriptor set avec la texture au binding 0
void UIRenderer::CreateUIDescriptorSet() {
	
	// Facilite la vie pour le reste de l'ecriture de la fonction (pas faire m_context.GetDevice() a chaque fois)
	auto device = m_context.GetDevice();

	// ---------- 1. Descriptor Pool ----------
	vk::DescriptorPoolSize poolSize{};

	// Le type est un sampler car on a une image
	poolSize.setType(
		vk::DescriptorType::eCombinedImageSampler
	);
	poolSize.setDescriptorCount(1);

	auto poolInfo = vk::DescriptorPoolCreateInfo{};
	poolInfo.setMaxSets(1);
	poolInfo.setPoolSizes(poolSize);

	m_uiDescriptorPool =
		device.createDescriptorPoolUnique(poolInfo);


	// ---------- 2. Allocation du Descriptor Set ----------
	auto allocateInfo = vk::DescriptorSetAllocateInfo{};

	allocateInfo.setDescriptorPool(*m_uiDescriptorPool);
	allocateInfo.setSetLayouts(*m_uiDescriptorSetLayout);

	m_uiDescriptorSet =
		device.allocateDescriptorSets(allocateInfo).front();


	// ---------- 3. Informations de notre texture ----------
	auto textureInfo = vk::DescriptorImageInfo{};

	textureInfo.setSampler(*m_pauseTexture.m_uiSampler);
	textureInfo.setImageView(*m_pauseTexture.m_uiImageView);
	textureInfo.setImageLayout(
		vk::ImageLayout::eShaderReadOnlyOptimal
	);


	// ---------- 4. Écriture du Descriptor Set ----------
	auto write = vk::WriteDescriptorSet{};

	write.setDstSet(m_uiDescriptorSet);
	write.setDstBinding(0);
	write.setDstArrayElement(0);
	write.setDescriptorType(
		vk::DescriptorType::eCombinedImageSampler
	);
	write.setImageInfo(textureInfo);

	device.updateDescriptorSets(write, {});
}



// Creer tout ce qui faut pour la texture
UIRenderer::VulkanTexture UIRenderer::CreateTexture(const std::string& path){
   
    VulkanTexture tex;

	// ---------- 1. Charger les pixels (RGBA, 8 bits) ----------
	int width = 0, height = 0, channels = 0;
	stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	if (!pixels)
		throw std::runtime_error("Impossible de charger la texture : " + path);

	// -- Chargement reussi, stocker dimensions textures -- // 
	tex.width = static_cast<uint32_t>(width);
	tex.height = static_cast<uint32_t>(height);


	vk::DeviceSize imageSize = static_cast<vk::DeviceSize>(width) * height * 4;
	auto device = m_context.GetDevice();

	// ---------- 2. Buffer intermédiaire (staging) visible par le CPU ----------
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

	void* data = device.mapMemory(*stagingMemory, 0, imageSize);
	std::memcpy(data, pixels, static_cast<size_t>(imageSize));
	device.unmapMemory(*stagingMemory);
	stbi_image_free(pixels);

	// ---------- 3. Image GPU ----------
	// Format sRGB : le GPU convertit en linéaire à la lecture, ce qui est correct avec une swapchain sRGB.
	auto imageInfo = vk::ImageCreateInfo{};
	imageInfo.setImageType(vk::ImageType::e2D);
	imageInfo.setFormat(vk::Format::eR8G8B8A8Srgb);
	imageInfo.setExtent({ static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 });
	imageInfo.setMipLevels(1);
	imageInfo.setArrayLayers(1);
	imageInfo.setSamples(vk::SampleCountFlagBits::e1);
	imageInfo.setTiling(vk::ImageTiling::eOptimal);
	imageInfo.setUsage(vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled);
	imageInfo.setInitialLayout(vk::ImageLayout::eUndefined);
	tex.m_uiImage = device.createImageUnique(imageInfo);

	auto imageReqs = device.getImageMemoryRequirements(*(tex.m_uiImage));
	auto imageAlloc = vk::MemoryAllocateInfo{};
	imageAlloc.setAllocationSize(imageReqs.size);
	imageAlloc.setMemoryTypeIndex(m_context.GetMemoryTypeIndex(
		imageReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal));
	tex.m_uiImageMemory = device.allocateMemoryUnique(imageAlloc);
	device.bindImageMemory(*(tex.m_uiImage), *(tex.m_uiImageMemory), 0);

	// ---------- 4. Copie staging -> image, avec changements de layout ----------
	auto cmdAlloc = vk::CommandBufferAllocateInfo{};
	cmdAlloc.setCommandPool(m_context.GetCommandPool());
	cmdAlloc.setLevel(vk::CommandBufferLevel::ePrimary);
	cmdAlloc.setCommandBufferCount(1);
	auto cmdBuffers = device.allocateCommandBuffersUnique(cmdAlloc);
	auto& cmd = cmdBuffers[0];

	auto beginInfo = vk::CommandBufferBeginInfo{};
	beginInfo.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
	cmd->begin(beginInfo);

	auto range = vk::ImageSubresourceRange{ vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 };

	// Undefined -> TransferDst
	auto toTransfer = vk::ImageMemoryBarrier{};
	toTransfer.setOldLayout(vk::ImageLayout::eUndefined);
	toTransfer.setNewLayout(vk::ImageLayout::eTransferDstOptimal);
	toTransfer.setSrcQueueFamilyIndex(vk::QueueFamilyIgnored);
	toTransfer.setDstQueueFamilyIndex(vk::QueueFamilyIgnored);
	toTransfer.setImage(*tex.m_uiImage);
	toTransfer.setSubresourceRange(range);
	toTransfer.setSrcAccessMask({});
	toTransfer.setDstAccessMask(vk::AccessFlagBits::eTransferWrite);
	cmd->pipelineBarrier(
		vk::PipelineStageFlagBits::eTopOfPipe,
		vk::PipelineStageFlagBits::eTransfer,
		{}, {}, {}, toTransfer);

	auto region = vk::BufferImageCopy{};
	region.setImageSubresource({ vk::ImageAspectFlagBits::eColor, 0, 0, 1 });
	region.setImageExtent({ static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 });
	cmd->copyBufferToImage(
		*stagingBuffer, *(tex.m_uiImage),
		vk::ImageLayout::eTransferDstOptimal, region);

	// TransferDst -> ShaderReadOnly
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
	m_context.GetQueue().waitIdle(); // le staging buffer est détruit à la fin de la fonction

	// ---------- 5. Image view ----------
	auto viewInfo = vk::ImageViewCreateInfo{};
	viewInfo.setImage(*(tex.m_uiImage));
	viewInfo.setViewType(vk::ImageViewType::e2D);
	viewInfo.setFormat(vk::Format::eR8G8B8A8Srgb);
	viewInfo.setSubresourceRange(range);
	tex.m_uiImageView = device.createImageViewUnique(viewInfo);

	// ---------- 6. Sampler : restreinte a l'espace pour l'UI ----------
	auto samplerInfo = vk::SamplerCreateInfo{};
	samplerInfo.setMagFilter(vk::Filter::eLinear);
	samplerInfo.setMinFilter(vk::Filter::eLinear);
	samplerInfo.setMipmapMode(vk::SamplerMipmapMode::eLinear);
	samplerInfo.setAddressModeU(vk::SamplerAddressMode::eClampToEdge);			// pour arreter la texture aux limites du PNG
	samplerInfo.setAddressModeV(vk::SamplerAddressMode::eClampToEdge);
	samplerInfo.setAddressModeW(vk::SamplerAddressMode::eClampToEdge);
	tex.m_uiSampler = device.createSamplerUnique(samplerInfo);

	// Print debug pour dire que la texture est chargee
	std::cout << "Texture UI chargee : " << tex.width << " x " << tex.height << std::endl;

    return tex;

}
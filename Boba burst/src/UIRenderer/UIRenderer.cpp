// -- UI_RENDERER_CPP -- => S'occupe de la création du pipeline 2D --- //

#include "UIRenderer.h"


// Definition constructeur
UIRenderer::UIRenderer(VulkanContext& context,
	vk::RenderPass renderpass) : m_context{ context } {
	CreateUIPipeline(renderpass);
}

void UIRenderer::Draw(vk::CommandBuffer commandBuffer, vk::Extent2D extent) {
    
    // Activer le pipeline UI
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_ui_pipeline);
    

    // Configurer le viewport (zone où on veut dessiner)
    vk::Viewport viewport{};

    viewport.setX(0.0f);
    viewport.setY(0.0f);
    viewport.setWidth(static_cast<float>(extent.width));
    viewport.setHeight(static_cast<float>(extent.height));
    viewport.setMinDepth(0.0f);
    viewport.setMaxDepth(1.0f);

    commandBuffer.setViewport(0, { viewport });

    // Configurer le scissor (zone où on peut dessiner)
    vk::Rect2D scissor{};

    scissor.setOffset({ 0, 0 });
    scissor.setExtent(extent);

    commandBuffer.setScissor(0, { scissor });


    // Enregistre commande dessin 2 triangles
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

    // Création layout vide car pas besoin de passer lumière ou texture aux shaders
    vk::PipelineLayoutCreateInfo layoutInfo{};

    m_ui_pipelineLayout =
        m_context.GetDevice().createPipelineLayoutUnique(layoutInfo);

    // Color attachement
    auto colorAttachmentBlendState = vk::PipelineColorBlendAttachmentState{};
    colorAttachmentBlendState.colorWriteMask =
        vk::ColorComponentFlagBits::eR |
        vk::ColorComponentFlagBits::eG |
        vk::ColorComponentFlagBits::eB |
        vk::ColorComponentFlagBits::eA;

    std::array<vk::PipelineColorBlendAttachmentState, 1> colorAttachmentBlendStates = { colorAttachmentBlendState };
    auto colorBlendState = vk::PipelineColorBlendStateCreateInfo{};
    colorBlendState.setAttachments(colorAttachmentBlendStates);

    // Pas encore de transparence pour le prototype
    colorAttachmentBlendState.setBlendEnable(VK_FALSE);

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
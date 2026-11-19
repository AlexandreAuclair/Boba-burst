#include "CubeScene.h"
#include <glm/vec3.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

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
		COLOR,
	};

	constexpr int g_vertexCount = 8;
	constexpr int g_indexCount = 36;
}

// clang-format off
static const VERTEX g_vertices[g_vertexCount] =
{
	{ {-1.0f,  1.0f, -1.0f}, 0xFF800000 },
	{ { 1.0f,  1.0f, -1.0f}, 0xFFFF0000 },
	{ { 1.0f,  1.0f,  1.0f}, 0xFFFF80FF },
	{ {-1.0f,  1.0f,  1.0f}, 0xFFFFFFFF },

	{ {-1.0f, -1.0f,  1.0f}, 0xFFFFFFFF },
	{ { 1.0f, -1.0f,  1.0f}, 0xFFFFFFFF },
	{ { 1.0f, -1.0f, -1.0f}, 0xFFFFFFFF },
	{ {-1.0f, -1.0f, -1.0f}, 0xFFFFFFFF },
};

static const uint16_t g_indices[g_indexCount] =
{
	0, 1, 2,
	0, 2, 3,

	4, 5, 6,
	4, 6, 7,

	3, 2, 5,
	3, 5, 4,

	2, 1, 6,
	2, 6, 5,

	1, 7, 6,
	1, 0, 7,

	0, 3, 4,
	0, 4, 7
};
// clang-format on

CubeScene::CubeScene(VulkanContext& context)
    : m_context(context)
{
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreatePipelineLayout();
}

void CubeScene::CreateVertexBuffer()
{
	auto bufferCreateInfo = vk::BufferCreateInfo{};
	bufferCreateInfo.setSize(sizeof(g_vertices));
	bufferCreateInfo.setUsage(vk::BufferUsageFlagBits::eVertexBuffer);
	m_vertexBuffer = m_context.GetDevice().createBufferUnique(bufferCreateInfo);

	auto memoryReqs = m_context.GetDevice().getBufferMemoryRequirements(*m_vertexBuffer);

	uint32_t memoryTypeIndex = m_context.GetMemoryTypeIndex(memoryReqs.memoryTypeBits,
	                                                        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eDeviceLocal);
	assert(memoryTypeIndex != -1);

	auto memoryAllocateInfo = vk::MemoryAllocateInfo{};
	memoryAllocateInfo.setAllocationSize(memoryReqs.size);
	memoryAllocateInfo.setMemoryTypeIndex(memoryTypeIndex);
	m_vertexBufferMemory = m_context.GetDevice().allocateMemoryUnique(memoryAllocateInfo);

	m_context.GetDevice().bindBufferMemory(*m_vertexBuffer, *m_vertexBufferMemory, {});

	{
		auto bufferPtr = m_context.GetDevice().mapMemory(*m_vertexBufferMemory, {}, VK_WHOLE_SIZE);
		memcpy(bufferPtr, g_vertices, sizeof(g_vertices));
		m_context.GetDevice().unmapMemory(*m_vertexBufferMemory);
	}
}

void CubeScene::CreateIndexBuffer()
{
	auto bufferCreateInfo = vk::BufferCreateInfo{};
	bufferCreateInfo.setSize(sizeof(g_indices));
	bufferCreateInfo.setUsage(vk::BufferUsageFlagBits::eIndexBuffer);
	m_indexBuffer = m_context.GetDevice().createBufferUnique(bufferCreateInfo);

	auto memoryReqs = m_context.GetDevice().getBufferMemoryRequirements(*m_indexBuffer);

	uint32_t memoryTypeIndex = m_context.GetMemoryTypeIndex(memoryReqs.memoryTypeBits,
	                                                        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eDeviceLocal);
	assert(memoryTypeIndex != -1);

	auto memoryAllocateInfo = vk::MemoryAllocateInfo{};
	memoryAllocateInfo.setAllocationSize(memoryReqs.size);
	memoryAllocateInfo.setMemoryTypeIndex(memoryTypeIndex);
	m_indexBufferMemory = m_context.GetDevice().allocateMemoryUnique(memoryAllocateInfo);

	m_context.GetDevice().bindBufferMemory(*m_indexBuffer, *m_indexBufferMemory, {});

	{
		auto bufferPtr = m_context.GetDevice().mapMemory(*m_indexBufferMemory, {}, VK_WHOLE_SIZE);
		memcpy(bufferPtr, g_indices, sizeof(g_indices));
		m_context.GetDevice().unmapMemory(*m_indexBufferMemory);
	}
}

void CubeScene::CreatePipelineLayout()
{
	auto pushConstantRange = vk::PushConstantRange{};
	pushConstantRange.setSize(sizeof(Matrices));
	pushConstantRange.setStageFlags(vk::ShaderStageFlagBits::eVertex);

	auto pipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo{};
	pipelineLayoutCreateInfo.setPushConstantRanges({pushConstantRange});
	m_pipelineLayout = m_context.GetDevice().createPipelineLayoutUnique(pipelineLayoutCreateInfo);
}

void CubeScene::CreatePipeline(vk::RenderPass renderPass)
{
	auto vsModule = m_context.CreateShaderModuleFromFile("./shaders_out/proj.vert.spv");
	auto psModule = m_context.CreateShaderModuleFromFile("./shaders_out/proj.frag.spv");

	std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages;
	shaderStages[0].setModule(*vsModule);
	shaderStages[0].setStage(vk::ShaderStageFlagBits::eVertex);
	shaderStages[0].setPName("main");

	shaderStages[1].setModule(*psModule);
	shaderStages[1].setStage(vk::ShaderStageFlagBits::eFragment);
	shaderStages[1].setPName("main");

	std::vector<vk::VertexInputAttributeDescription> vertexAttributes;

	//Position Attribute
	{
		auto vertexAttribute = vk::VertexInputAttributeDescription{};
		vertexAttribute.setBinding(0);
		vertexAttribute.setLocation(POSITION);
		vertexAttribute.setFormat(vk::Format::eR32G32B32Sfloat);
		vertexAttribute.setOffset(offsetof(VERTEX, position));
		vertexAttributes.push_back(vertexAttribute);
	}

	//Color Attribute
	{
		auto vertexAttribute = vk::VertexInputAttributeDescription{};
		vertexAttribute.setBinding(0);
		vertexAttribute.setLocation(COLOR);
		vertexAttribute.setFormat(vk::Format::eR8G8B8A8Unorm);
		vertexAttribute.setOffset(offsetof(VERTEX, color));
		vertexAttributes.push_back(vertexAttribute);
	}

	auto vertexBinding = vk::VertexInputBindingDescription{};
	vertexBinding.setInputRate(vk::VertexInputRate::eVertex);
	vertexBinding.setStride(sizeof(VERTEX));

	auto vertexInputState = vk::PipelineVertexInputStateCreateInfo{};
	vertexInputState.setVertexAttributeDescriptions(vertexAttributes);
	vertexInputState.setVertexBindingDescriptions(vertexBinding);

	auto inputAssemblyState = vk::PipelineInputAssemblyStateCreateInfo{};
	inputAssemblyState.setTopology(vk::PrimitiveTopology::eTriangleList);

	auto rasterizationState = vk::PipelineRasterizationStateCreateInfo{};
	rasterizationState.setLineWidth(1.0f);
	rasterizationState.setCullMode(vk::CullModeFlagBits::eBack);

	auto viewportState = vk::PipelineViewportStateCreateInfo{};
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	auto depthStencilState = vk::PipelineDepthStencilStateCreateInfo{};
	depthStencilState.setDepthWriteEnable(VK_TRUE);
	depthStencilState.setDepthTestEnable(VK_TRUE);
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
	m_pipeline = std::move(result.value);
}

void CubeScene::Update(double dt)
{
	Scene::Update(dt);

	auto surfaceExtents = m_context.GetSurfaceExtents();
	float aspectRatio = static_cast<float>(surfaceExtents.width) / static_cast<float>(surfaceExtents.height);

	glm::mat4 projMat = glm::perspective(glm::pi<float>() * 0.25f, aspectRatio, 0.1f, 1000.f);
	glm::mat4 viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f));
	glm::mat4 worldMat = glm::rotate(glm::mat4(1.0f), static_cast<float>(m_currentTime * 2), glm::vec3(0.5f, 1.0f, 0.5f));

	m_matrices.worldViewProjMatrix = projMat * viewMat * worldMat;
}

void CubeScene::Draw(const DrawInfo& drawInfo)
{
	if(!m_pipeline)
	{
		CreatePipeline(drawInfo.renderPass);
	}

	drawInfo.commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_pipeline);
	drawInfo.commandBuffer.bindVertexBuffers(0, {*m_vertexBuffer}, {0});
	drawInfo.commandBuffer.bindIndexBuffer(*m_indexBuffer, 0, vk::IndexType::eUint16);
	drawInfo.commandBuffer.pushConstants(*m_pipelineLayout, vk::ShaderStageFlagBits::eVertex,
	                                     0, sizeof(m_matrices), &m_matrices);
	drawInfo.commandBuffer.drawIndexed(g_indexCount, 1, 0, 0, 0);
}

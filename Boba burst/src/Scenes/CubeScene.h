#pragma once

#include <glm/mat4x4.hpp>
#include "../VulkanContext/bb_vulkanContext.h"
#include "bb_scene.h"

class CubeScene : public Scene
{
public:
	CubeScene(VulkanContext&);

	void Update(double) override;
	void Draw(const DrawInfo&) override;

private:
	struct Matrices
	{
		glm::mat4 worldViewProjMatrix;
	};

	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreatePipelineLayout();
	void CreatePipeline(vk::RenderPass);

	VulkanContext& m_context;
	Matrices m_matrices = {};
	vk::UniqueBuffer m_vertexBuffer;
	vk::UniqueDeviceMemory m_vertexBufferMemory;
	vk::UniqueBuffer m_indexBuffer;
	vk::UniqueDeviceMemory m_indexBufferMemory;

	vk::UniquePipelineLayout m_pipelineLayout;
	vk::UniquePipeline m_pipeline;
};

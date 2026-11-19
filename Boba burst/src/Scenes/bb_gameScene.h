#pragma once

#include <glm/mat4x4.hpp>
#include "../VulkanContext/bb_vulkanContext.h"
#include "../DataStruct/bb_lightData.h"
#include "../Game/bb_gameManager.h"
#include "bb_scene.h"

class GameScene : public Scene
{

public:
    GameScene(VulkanContext& context);

	void Update(double dt) override;
	void Draw(const DrawInfo& drawInfo) override;

private:
	struct Matrices
	{
		glm::mat4 worldViewProjMatrix;
		glm::mat4 worldMatrix;
	};

	void LoadMesh(const std::string& meshPath);
    void CreateAndUploadBuffer(const void* srcData, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::UniqueBuffer& outBuffer, vk::UniqueDeviceMemory& outMemory);
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreatePipelineLayout();

	// Pipeline 3D
	void CreatePipeline(vk::RenderPass renderPass);


	VulkanContext& m_context;

	// Pour placer mon terrain devant ma cam�ra
	glm::vec3 m_center{ 0.0f };
	float m_scale = 1.0f;

	Matrices m_matrices = {};

	vk::UniqueBuffer m_vertexBuffer;
	vk::UniqueDeviceMemory m_vertexBufferMemory;

	vk::UniqueBuffer m_indexBuffer;
	vk::UniqueDeviceMemory m_indexBufferMemory;

	//////////////
	// Sert pour la 3D
	vk::UniquePipelineLayout m_pipelineLayout;				// Layout Pipeline rendu 3D
	vk::UniquePipeline m_pipeline;							// Pipeline rendu 3D
	//////////////

	LightData m_lightData{};
	void* m_lightBufferMapped = nullptr; // memoire mappee en permanence

	vk::UniqueBuffer m_lightBuffer;
	vk::UniqueDeviceMemory m_lightBufferMemory;

	vk::UniqueDescriptorSetLayout m_lightDescriptorSetLayout;
	vk::UniqueDescriptorPool m_lightDescriptorPool;
	vk::DescriptorSet m_lightDescriptorSet{};

	void CreateLightResources();
	void UpdateLightBuffer(const glm::vec3& cameraPosition);


	// ---------- Donnees pour les textures ----------
private: 

	// Prend le path de la texture et cr�� texture utilisable par Vulkain 
	void SendTextureToGPU(Texture& tex);

    vk::DescriptorPool *m_descriptorPool;
    vk::DescriptorSetLayout *m_descriptorSetLayout;
    
};


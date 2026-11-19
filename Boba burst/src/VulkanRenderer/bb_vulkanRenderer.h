#pragma once

#include <memory>

#include "../Scenes/bb_scene.h"
#include "../VulkanContext/bb_vulkanContext.h"
#include "../UIRenderer/UIRenderer.h"

class VulkanRenderer
{
public:
    VulkanRenderer(VulkanContext&);
    VulkanRenderer(VulkanRenderer&&) = delete;

	VulkanRenderer& operator=(VulkanRenderer&&) = delete;

	void SetScene(std::shared_ptr<Scene>);

	void Draw(bool showMenuPause);
	void BeginFrame();
	void EndFrame();

private:
	struct PerFrameInfo
	{
		vk::UniqueFence doneFence;
		vk::UniqueCommandBuffer commandBuffer;
		//TODO: Descriptor pool
	};

	struct SWAPCHAIN_SEMAPAIR
	{
		vk::UniqueSemaphore imageAcquireSema;
		vk::UniqueSemaphore renderCompleteSema;
	};

	void CreatePerFrameInfo();
	void CreateDepthbuffer();
	void CreateRenderPass();
	void CreateFramebuffers();

	// Creer pipeline graphique 2D pour l'UI
	std::unique_ptr<UIRenderer> m_uiRenderer;

	VulkanContext& m_context;

	vk::UniqueImage m_depthbufferImage;
	vk::UniqueDeviceMemory m_depthbufferImageMemory;
	vk::UniqueImageView m_depthbufferImageView;

	vk::UniqueRenderPass m_renderPass;
	std::vector<vk::UniqueFramebuffer> m_framebuffers;

	std::array<SWAPCHAIN_SEMAPAIR, 5> m_swapChainSemaPairs;
	int m_currentSwapChainSemaPair = 0;

	std::array<PerFrameInfo, 2> m_perFrameInfos;
	int m_currentFrameIndex = 0;
	uint32_t m_currentSwapChainImageIndex = -1;

	std::shared_ptr<Scene> m_scene;
};
#pragma once

#include <span>

#include "../DataStruct/VulkanDef.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class VulkanContext
{
public:
	VulkanContext() = default;
	VulkanContext(VulkanContext &&) = delete;

	VulkanContext &operator=(VulkanContext &&) = delete;

	void CreateInstance();
	void CreateDevice();
	void CreateSwapChain();
	void CleanUpSwapChain();

	void TransitionColorImage(vk::Image, vk::ImageLayout, vk::ImageLayout);

	vk::Instance GetInstance() const
	{
		return *m_instance;
	}

	vk::Device GetDevice() const
	{
		return *m_device;
	}

	vk::Queue GetQueue() const
	{
		return m_queue;
	}

	vk::CommandPool GetCommandPool() const
	{
		return *m_commandPool;
	}

	void SetSurface(VkSurfaceKHR surface)
	{
		m_surface = vk::UniqueSurfaceKHR(surface, *m_instance);
	}

	vk::Extent2D GetSurfaceExtents() const
	{
		return m_surfaceExtents;
	}

	vk::SwapchainKHR GetSwapChain() const
	{
		return *m_swapChain;
	}

	vk::Format GetSwapChainImageFormat() const
	{
		return m_swapChainImageFormat;
	}

	std::span<const vk::UniqueImageView> GetSwapChainImageViews() const
	{
		return m_swapChainImageViews;
	}

	uint32_t GetMemoryTypeIndex(uint32_t, vk::MemoryPropertyFlags) const;
	vk::UniqueShaderModule CreateShaderModuleFromFile(const char *) const;

	GLFWwindow* window;

private:
	void CreateSwapChainImageViews();

	vk::UniqueInstance m_instance;
	vk::PhysicalDevice m_physicalDevice;
	vk::PhysicalDeviceMemoryProperties m_memoryProperties;
	vk::UniqueDevice m_device;
	vk::Queue m_queue;

	vk::UniqueCommandPool m_commandPool;

	vk::UniqueSurfaceKHR m_surface;
	vk::Extent2D m_surfaceExtents = {};

	vk::UniqueSwapchainKHR m_swapChain;
	vk::Format m_swapChainImageFormat = vk::Format::eR8G8B8A8Unorm;
	std::vector<vk::Image> m_swapChainImages;
	std::vector<vk::UniqueImageView> m_swapChainImageViews;
	bool m_swapChainValid = false;
};

#include "bb_vulkanContext.h"
#include <cstdio>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

void VulkanContext::CreateInstance()
{
	const char *layers[] = {"VK_LAYER_KHRONOS_validation"};

	uint32_t glfwExtensionCount = 0;
	const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	// Add any extra extensions you need, like validation debug utils:
	extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	auto appInfo = vk::ApplicationInfo{};
	appInfo.apiVersion = VK_API_VERSION_1_3;

	auto instanceCreateInfo = vk::InstanceCreateInfo{};
	instanceCreateInfo.setPApplicationInfo(&appInfo);
	instanceCreateInfo.setPEnabledLayerNames(layers);
	instanceCreateInfo.setPEnabledExtensionNames(extensions);
#ifdef __APPLE__
	instanceCreateInfo.setFlags(vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR);
#endif

	m_instance = vk::createInstanceUnique(instanceCreateInfo);
}

void VulkanContext::CreateDevice()
{
	constexpr uint32_t queueFamilyIndex = 0;

	auto physicalDevices = m_instance->enumeratePhysicalDevices();
	assert(!physicalDevices.empty());
	m_physicalDevice = physicalDevices[0];

	const char *extensions[] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME};

	float queuePriorities[] = {1.0f};

	auto deviceQueueCreateInfo = vk::DeviceQueueCreateInfo{};
	deviceQueueCreateInfo.setQueueFamilyIndex(queueFamilyIndex);
	deviceQueueCreateInfo.setQueueCount(1);
	deviceQueueCreateInfo.setPQueuePriorities(queuePriorities);

	auto deviceCreateInfo = vk::DeviceCreateInfo{};
	deviceCreateInfo.setPEnabledExtensionNames(extensions);
	deviceCreateInfo.setQueueCreateInfos(deviceQueueCreateInfo);
	m_device = m_physicalDevice.createDeviceUnique(deviceCreateInfo);

	m_memoryProperties = m_physicalDevice.getMemoryProperties();

	m_queue = m_device->getQueue(queueFamilyIndex, 0);

	{
		auto commandPoolCreateInfo = vk::CommandPoolCreateInfo{};
		commandPoolCreateInfo.setQueueFamilyIndex(queueFamilyIndex);
		commandPoolCreateInfo.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
		m_commandPool = m_device->createCommandPoolUnique(commandPoolCreateInfo);
	}
}

void VulkanContext::CreateSwapChain()
{
	assert(m_surface);

	auto surfaceCaps = m_physicalDevice.getSurfaceCapabilitiesKHR(*m_surface);
	m_surfaceExtents = surfaceCaps.currentExtent;

	// 1. Dynamically determine image count (respecting min and max limits)
	uint32_t imageCount = surfaceCaps.minImageCount + 1;
	if (surfaceCaps.maxImageCount > 0 && imageCount > surfaceCaps.maxImageCount)
	{
		imageCount = surfaceCaps.maxImageCount;
	}

	// 2. Query supported surface formats instead of hardcoding
	auto formats = m_physicalDevice.getSurfaceFormatsKHR(*m_surface);
	assert(!formats.empty());

	vk::SurfaceFormatKHR surfaceFormat = formats[0];
	for (const auto &fmt : formats)
	{
		// Prefer B8G8R8A8_UNORM with sRGB color space if available
		if (fmt.format == vk::Format::eB8G8R8A8Unorm && fmt.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
		{
			surfaceFormat = fmt;
			break;
		}
	}
	m_swapChainImageFormat = surfaceFormat.format;

	auto swapChainCreateInfo = vk::SwapchainCreateInfoKHR{};
	swapChainCreateInfo.setSurface(*m_surface);
	swapChainCreateInfo.setMinImageCount(imageCount); // Use dynamic count
	swapChainCreateInfo.setImageFormat(m_swapChainImageFormat);
	swapChainCreateInfo.setImageColorSpace(surfaceFormat.colorSpace); // Use queried color space
	swapChainCreateInfo.setImageExtent(m_surfaceExtents);
	swapChainCreateInfo.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);
	swapChainCreateInfo.setPreTransform(surfaceCaps.currentTransform); // Match current transform
	swapChainCreateInfo.setImageArrayLayers(1);
	swapChainCreateInfo.setImageSharingMode(vk::SharingMode::eExclusive);
	swapChainCreateInfo.setPresentMode(vk::PresentModeKHR::eFifo);
	swapChainCreateInfo.setClipped(VK_TRUE);
	swapChainCreateInfo.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque);

	m_swapChain = m_device->createSwapchainKHRUnique(swapChainCreateInfo);
	m_swapChainImages = m_device->getSwapchainImagesKHR(*m_swapChain);

	CreateSwapChainImageViews();
	m_swapChainValid = true;
}

void VulkanContext::CleanUpSwapChain()
{
	m_swapChainImageViews.clear();
	m_swapChainImages.clear();
	m_swapChain.reset();
}

void VulkanContext::CreateSwapChainImageViews()
{
	for (const auto &image : m_swapChainImages)
	{
		auto imageViewCreateInfo = vk::ImageViewCreateInfo{};
		imageViewCreateInfo.setFormat(m_swapChainImageFormat);
		imageViewCreateInfo.setViewType(vk::ImageViewType::e2D);
		imageViewCreateInfo.image = image;
		imageViewCreateInfo.setComponents(
			{vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG,
			 vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA});
		imageViewCreateInfo.setSubresourceRange({vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});

		auto imageView = m_device->createImageViewUnique(imageViewCreateInfo);
		m_swapChainImageViews.push_back(std::move(imageView));
	}
}

void VulkanContext::TransitionColorImage(vk::Image image, vk::ImageLayout srcImageLayout, vk::ImageLayout dstImageLayout)
{
	auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo{};
	commandBufferAllocateInfo.setCommandPool(*m_commandPool);
	commandBufferAllocateInfo.setCommandBufferCount(1);
	commandBufferAllocateInfo.setLevel(vk::CommandBufferLevel::ePrimary);
	auto commandBuffers = m_device->allocateCommandBuffersUnique(commandBufferAllocateInfo);

	auto commandBuffer = *commandBuffers[0];

	auto imageMemoryBarrier = vk::ImageMemoryBarrier{};
	imageMemoryBarrier.setImage(image);
	imageMemoryBarrier.setOldLayout(srcImageLayout);
	imageMemoryBarrier.setNewLayout(dstImageLayout);
	imageMemoryBarrier.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
	imageMemoryBarrier.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
	imageMemoryBarrier.setSubresourceRange({vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});

	auto commandBufferBeginInfo = vk::CommandBufferBeginInfo{};
	commandBufferBeginInfo.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

	commandBuffer.begin(commandBufferBeginInfo);
	commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eBottomOfPipe,
								  {}, {}, {}, {imageMemoryBarrier});
	commandBuffer.end();

	vk::SubmitInfo submitInfo = {};
	submitInfo.setCommandBuffers(commandBuffer);
	m_queue.submit(submitInfo);
	m_queue.waitIdle();
}

uint32_t VulkanContext::GetMemoryTypeIndex(uint32_t deviceRequirements, vk::MemoryPropertyFlags hostRequirements) const
{
	for (uint32_t i = 0; i < VK_MAX_MEMORY_TYPES; i++)
	{
		if (deviceRequirements & 1 << i)
		{
			const auto &memoryType = m_memoryProperties.memoryTypes[i];
			if ((memoryType.propertyFlags & hostRequirements) == hostRequirements)
			{
				return i;
			}
		}
	}
	return -1;
}

vk::UniqueShaderModule VulkanContext::CreateShaderModuleFromFile(const char *modulePath) const
{
	FILE *inputStream = fopen(modulePath, "rb");
	assert(inputStream);
	fseek(inputStream, 0, SEEK_END);
	uint64_t shaderLength = ftell(inputStream);
	fseek(inputStream, 0, SEEK_SET);
	assert((shaderLength & 0x03) == 0);

	std::vector<uint32_t> shader(shaderLength / 4);
	uint64_t read = fread(shader.data(), 1, shaderLength, inputStream);
	assert(read == shaderLength);

	fclose(inputStream);

	auto shaderModuleCreateInfo = vk::ShaderModuleCreateInfo{};
	shaderModuleCreateInfo.codeSize = shaderLength;
	shaderModuleCreateInfo.pCode = shader.data();
	return m_device->createShaderModuleUnique(shaderModuleCreateInfo);
}

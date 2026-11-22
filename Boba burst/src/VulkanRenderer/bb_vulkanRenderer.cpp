#include "bb_vulkanRenderer.h"

VulkanRenderer::VulkanRenderer(VulkanContext &context)
	: m_context(context)
{
	CreateDepthbuffer();
	CreateRenderPass();

	// Le render pass existe ici : on peut construire le pipeline UI
	m_uiRenderer = std::make_unique<UIRenderer>(m_context, *m_renderPass);

	CreateFramebuffers();
	CreatePerFrameInfo();
	for (auto &semaPair : m_swapChainSemaPairs)
	{
		semaPair.imageAcquireSema = context.GetDevice().createSemaphoreUnique({});
		semaPair.renderCompleteSema = context.GetDevice().createSemaphoreUnique({});
	}
}

void VulkanRenderer::CreateRenderPass()
{
	vk::AttachmentReference colorRef = {};
	colorRef.setAttachment(0);
	colorRef.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

	vk::AttachmentReference depthRef = {};
	depthRef.setAttachment(1);
	depthRef.setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

	vk::SubpassDescription subpass = {};
	subpass.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);
	subpass.setColorAttachments(colorRef);
	subpass.setPDepthStencilAttachment(&depthRef);

	vk::AttachmentDescription colorAttachment = {};
	colorAttachment.setFormat(m_context.GetSwapChainImageFormat());
	colorAttachment.setSamples(vk::SampleCountFlagBits::e1);
	colorAttachment.setLoadOp(vk::AttachmentLoadOp::eClear);
	colorAttachment.setStoreOp(vk::AttachmentStoreOp::eStore);
	colorAttachment.setInitialLayout(vk::ImageLayout::eUndefined);
	colorAttachment.setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

	vk::AttachmentDescription depthAttachment = {};
	depthAttachment.setFormat(vk::Format::eD32Sfloat);
	depthAttachment.setSamples(vk::SampleCountFlagBits::e1);
	depthAttachment.setLoadOp(vk::AttachmentLoadOp::eClear);
	depthAttachment.setStoreOp(vk::AttachmentStoreOp::eDontCare);
	depthAttachment.setInitialLayout(vk::ImageLayout::eUndefined);
	depthAttachment.setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

	auto attachments = {colorAttachment, depthAttachment};

	vk::RenderPassCreateInfo createInfo = {};
	createInfo.setAttachments(attachments);
	createInfo.setSubpasses({subpass});
	m_renderPass = m_context.GetDevice().createRenderPassUnique(createInfo);
}

void VulkanRenderer::CreateFramebuffers()
{
	auto swapChainImageViews = m_context.GetSwapChainImageViews();

	auto surfaceExtents = m_context.GetSurfaceExtents();
	m_framebuffers.resize(swapChainImageViews.size());
	for (int i = 0; i < swapChainImageViews.size(); i++)
	{
		auto attachments = {*swapChainImageViews[i], *m_depthbufferImageView};
		auto framebufferCreateInfo = vk::FramebufferCreateInfo{};
		framebufferCreateInfo.setAttachments(attachments);
		framebufferCreateInfo.setRenderPass(*m_renderPass);
		framebufferCreateInfo.setWidth(surfaceExtents.width);
		framebufferCreateInfo.setHeight(surfaceExtents.height);
		framebufferCreateInfo.setLayers(1);
		m_framebuffers[i] = m_context.GetDevice().createFramebufferUnique(framebufferCreateInfo);
	}
}

void VulkanRenderer::RecreateSwapChain()
{

	int width = 0;
	int height = 0;

	glfwGetFramebufferSize(m_context.window, &width, &height);

	// Attendre que la fenêtre retrouve une taille valide
	while (width == 0 || height == 0)
	{
		if (glfwWindowShouldClose(m_context.window))
			return;

		glfwWaitEvents();
		glfwGetFramebufferSize(m_context.window, &width, &height);
	}

	m_context.GetDevice().waitIdle();

	m_framebuffers.clear();

	// Détruire le depth buffer
	// Détruire les ressources de profondeur
	m_depthbufferImageView.reset();
	m_depthbufferImage.reset();
	m_depthbufferImageMemory.reset();

	m_context.CleanUpSwapChain();

	m_context.CreateSwapChain();

	CreateDepthbuffer();
	CreateFramebuffers();
}

void VulkanRenderer::CreatePerFrameInfo()
{
	auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo{};
	commandBufferAllocateInfo.setCommandBufferCount(m_perFrameInfos.size());
	commandBufferAllocateInfo.setLevel(vk::CommandBufferLevel::ePrimary);
	commandBufferAllocateInfo.setCommandPool(m_context.GetCommandPool());
	auto commandBuffers = m_context.GetDevice().allocateCommandBuffersUnique(commandBufferAllocateInfo);

	for (int i = 0; i < m_perFrameInfos.size(); i++)
	{
		auto &perFrameInfo = m_perFrameInfos[i];

		{
			auto fenceCreateInfo = vk::FenceCreateInfo{};
			fenceCreateInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);
			perFrameInfo.doneFence = m_context.GetDevice().createFenceUnique(fenceCreateInfo);
		}

		perFrameInfo.commandBuffer = std::move(commandBuffers[i]);
	}
}

void VulkanRenderer::CreateDepthbuffer()
{
	auto surfaceExtents = m_context.GetSurfaceExtents();

	{
		auto imageCreateInfo = vk::ImageCreateInfo();
		imageCreateInfo.setImageType(vk::ImageType::e2D);
		imageCreateInfo.setFormat(vk::Format::eD32Sfloat);
		imageCreateInfo.setUsage(vk::ImageUsageFlagBits::eDepthStencilAttachment);
		imageCreateInfo.setExtent({surfaceExtents.width, surfaceExtents.height, 1});
		imageCreateInfo.setMipLevels(1);
		imageCreateInfo.setArrayLayers(1);
		imageCreateInfo.setInitialLayout(vk::ImageLayout::eUndefined);
		m_depthbufferImage = m_context.GetDevice().createImageUnique(imageCreateInfo);
	}

	{
		auto memoryReqs = m_context.GetDevice().getImageMemoryRequirements(*m_depthbufferImage);

		uint32_t memoryTypeIndex = m_context.GetMemoryTypeIndex(memoryReqs.memoryTypeBits,
																vk::MemoryPropertyFlagBits::eDeviceLocal);
		assert(memoryTypeIndex != -1);

		auto memoryAllocateInfo = vk::MemoryAllocateInfo{};
		memoryAllocateInfo.setAllocationSize(memoryReqs.size);
		memoryAllocateInfo.setMemoryTypeIndex(memoryTypeIndex);
		m_depthbufferImageMemory = m_context.GetDevice().allocateMemoryUnique(memoryAllocateInfo);

		m_context.GetDevice().bindImageMemory(*m_depthbufferImage, *m_depthbufferImageMemory, 0);
	}

	{
		auto imageViewCreateInfo = vk::ImageViewCreateInfo();
		imageViewCreateInfo.setViewType(vk::ImageViewType::e2D);
		imageViewCreateInfo.setImage(*m_depthbufferImage);
		imageViewCreateInfo.setFormat(vk::Format::eD32Sfloat);
		imageViewCreateInfo.setSubresourceRange({vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1});
		m_depthbufferImageView = m_context.GetDevice().createImageViewUnique(imageViewCreateInfo);
	}
}

void VulkanRenderer::SetScene(std::shared_ptr<Scene> scene)
{
	m_scene = scene;
}

void VulkanRenderer::Draw(bool showMenuPause)
{
	BeginFrame();

	auto surfaceExtents = m_context.GetSurfaceExtents();
	auto &perFrameInfo = m_perFrameInfos[m_currentFrameIndex];

	{
		auto colorClearValue = vk::ClearValue{};
		colorClearValue.setColor({0.0f, 0.0f, 0.0f, 0.0f});

		auto depthClearValue = vk::ClearValue{};
		depthClearValue.setDepthStencil({1, 0});

		auto clearValues = {colorClearValue, depthClearValue};

		auto beginInfo = vk::RenderPassBeginInfo{};
		beginInfo.setRenderPass(*m_renderPass);
		beginInfo.setFramebuffer(*m_framebuffers[m_currentSwapChainImageIndex]);
		beginInfo.renderArea.extent.setWidth(surfaceExtents.width);
		beginInfo.renderArea.extent.setHeight(surfaceExtents.height);
		beginInfo.setClearValues(clearValues);

		perFrameInfo.commandBuffer->beginRenderPass(beginInfo, vk::SubpassContents::eInline);
	}

	auto viewport = vk::Viewport{};
	viewport.setWidth(surfaceExtents.width);
	viewport.setHeight(surfaceExtents.height);
	viewport.setMinDepth(0);
	viewport.setMaxDepth(1);
	perFrameInfo.commandBuffer->setViewport(0, {viewport});

	auto scissor = vk::Rect2D{};
	scissor.setExtent(surfaceExtents);
	perFrameInfo.commandBuffer->setScissor(0, {scissor});

	// ==========================================================
	// 1. RENDU DE LA SCÈNE 3D
	// ==========================================================
	if (m_scene)
	{
		DrawInfo drawInfo = {};
		drawInfo.commandBuffer = *perFrameInfo.commandBuffer;
		drawInfo.renderPass = *m_renderPass;
		m_scene->Draw(drawInfo);
	}

	// ==========================================================
	// 2. RENDU DE L'INTERFACE UTILISATEUR (UI du menu principal en dehors de présence scène 3D)
	// ==========================================================
	if (showMenuPause && m_uiRenderer)
	{
		m_uiRenderer->Draw(
			*perFrameInfo.commandBuffer,
			surfaceExtents);
	}

	// ==========================================================
	// 3. FIN DU RENDER PASS
	// ==========================================================
	perFrameInfo.commandBuffer->endRenderPass();

	EndFrame();
}

void VulkanRenderer::BeginFrame()
{
	auto &perFrameInfo = m_perFrameInfos[m_currentFrameIndex];
	auto &swapChainSemaPair = m_swapChainSemaPairs[m_currentSwapChainSemaPair];

	m_context.GetDevice().waitForFences({ *perFrameInfo.doneFence }, vk::True, UINT64_MAX);

	try
	{
		vk::ResultValue result = m_context.GetDevice().acquireNextImageKHR(m_context.GetSwapChain(),
																		   UINT64_MAX, *swapChainSemaPair.imageAcquireSema);

		m_currentSwapChainImageIndex = result.value;
	}
	catch (const vk::OutOfDateKHRError &)
	{
		RecreateSwapChain();
		return;
	}

	m_context.GetDevice().resetFences({*perFrameInfo.doneFence});
	perFrameInfo.commandBuffer->reset(vk::CommandBufferResetFlagBits::eReleaseResources);
	// TODO: Reset descriptor pool

	vk::CommandBufferBeginInfo beginInfo = {};
	beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
	perFrameInfo.commandBuffer->begin(beginInfo);
}

void VulkanRenderer::EndFrame()
{
	auto &perFrameInfo = m_perFrameInfos[m_currentFrameIndex];
	auto &swapChainSemaPair = m_swapChainSemaPairs[m_currentSwapChainSemaPair];

	perFrameInfo.commandBuffer->end();

	{
		std::array<vk::PipelineStageFlags, 1> waitDstStages = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

		vk::SubmitInfo submitInfo = {};
		submitInfo.setCommandBuffers({*perFrameInfo.commandBuffer});
		submitInfo.setWaitSemaphores({*swapChainSemaPair.imageAcquireSema});
		submitInfo.setWaitDstStageMask(waitDstStages);
		submitInfo.setSignalSemaphores({*swapChainSemaPair.renderCompleteSema});
		m_context.GetQueue().submit(submitInfo, *perFrameInfo.doneFence);
	}

	// Queue present
	{
		auto swapChains = {m_context.GetSwapChain()};
		auto presentInfo = vk::PresentInfoKHR{};
		presentInfo.setSwapchains(swapChains);
		presentInfo.setImageIndices({m_currentSwapChainImageIndex});
		presentInfo.setWaitSemaphores({*swapChainSemaPair.renderCompleteSema});

		try
		{
			m_context.GetQueue().presentKHR(presentInfo);
		}
		catch (const vk::OutOfDateKHRError&)
		{
			RecreateSwapChain();
		}
	}

	m_currentSwapChainSemaPair++;
	m_currentSwapChainSemaPair %= m_swapChainSemaPairs.size();

	m_currentFrameIndex++;
	m_currentFrameIndex %= m_perFrameInfos.size();
}

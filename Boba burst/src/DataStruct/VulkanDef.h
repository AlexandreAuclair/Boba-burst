#pragma once

#include <vulkan/vulkan.hpp>

struct DrawInfo
{
    vk::CommandBuffer commandBuffer;
    vk::RenderPass renderPass;
};
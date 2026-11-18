#pragma once 

#include <vulkan/vulkan.hpp>

struct Texture{
    int width;
    int height;
    int channels;
    unsigned char *pixels;

    // ressource Vulkan
    vk::UniqueImage image;
    vk::UniqueDeviceMemory memory;
    vk::UniqueImageView view;
    vk::UniqueSampler sampler;
    vk::DescriptorSet descriptorSet;
};
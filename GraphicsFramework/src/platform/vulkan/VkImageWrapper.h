#pragma once

#include "VkBufferWrapper.h"

class VkImageWrapper
{
public:
	VkImageWrapper(VkImage image);
	VkImageWrapper(unsigned int width, unsigned int height, VkSampleCountFlagBits numSamples, VkImageTiling tiling, VkImageUsageFlags usage, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels, VkMemoryPropertyFlags properties);
	virtual ~VkImageWrapper();

	VkImage GetImage() const;
	VkDeviceMemory GetImageMemory() const;

	void TransitionImageLayout(VkImageLayout newLayout);

private:
	VkImage m_image;
	VkDeviceMemory m_image_memory;
	VkImageLayout m_layout;
	VkFormat m_format;
	bool m_should_delete_images;
};
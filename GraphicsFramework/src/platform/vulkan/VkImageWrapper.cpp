#include "stdafx.h"

#include "VkImageWrapper.h"
#include "VkHelperMethods.h"

#include "VkContext.h"
#include "VkCommandList.h"

#include "graphics/BufferHelpers.h"

namespace
{
	bool HasStencilComponent(VkFormat format)
	{
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}
}

VkImageWrapper::VkImageWrapper(VkImage image):
	m_image(image)
{
	m_image_memory = VK_NULL_HANDLE;
	m_should_delete_images = false;
}

VkImageWrapper::VkImageWrapper(unsigned int width, 
	unsigned int height, 
	VkSampleCountFlagBits numSamples, 
	VkImageTiling tiling, 
	VkImageUsageFlags usage, 
	VkFormat format, 
	VkImageAspectFlags aspectFlags, 
	uint32_t mipLevels, 
	VkMemoryPropertyFlags properties)
{
	VkImageCreateInfo image_info = {};
	image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image_info.imageType = VK_IMAGE_TYPE_2D;
	image_info.extent.width = width;
	image_info.extent.height = height;
	image_info.extent.depth = 1;
	image_info.arrayLayers = 1;
	image_info.format = format;
	image_info.tiling = tiling;
	image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	image_info.usage = usage;
	image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	image_info.samples = numSamples;
	image_info.flags = 0;
	image_info.mipLevels = mipLevels;

	VKCALL(vkCreateImage(GetVkDevice(), &image_info, GetVkAllocationCallbacks(), &m_image));

	VkMemoryRequirements mem_requirements;
	vkGetImageMemoryRequirements(GetVkDevice(), m_image, &mem_requirements);

	VkMemoryAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	alloc_info.allocationSize = mem_requirements.size;
	alloc_info.memoryTypeIndex = FindMemoryType(mem_requirements.memoryTypeBits, properties);

	VKCALL(vkAllocateMemory(GetVkDevice(), &alloc_info, GetVkAllocationCallbacks(), &m_image_memory));
	VKCALL(vkBindImageMemory(GetVkDevice(), m_image, m_image_memory, 0));

	m_format = format;
	m_layout = VK_IMAGE_LAYOUT_UNDEFINED;
	m_should_delete_images = true;
}

VkImageWrapper::~VkImageWrapper()
{
	if (m_should_delete_images)
	{
		vkDestroyImage(GetVkDevice(), m_image, GetVkAllocationCallbacks());
		vkFreeMemory(GetVkDevice(), m_image_memory, GetVkAllocationCallbacks());
	}
}

void VkImageWrapper::TransitionImageLayout(VkImageLayout newLayout)
{
	std::unique_ptr<VkCommandList> command_list = GetVkContext()->CreateDirectCommandList();

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = m_layout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = m_image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = 0;
	if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		if (HasStencilComponent(m_format))
			barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
	}
	else
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

	VkPipelineStageFlags source_stage;
	VkPipelineStageFlags destination_stage;

	if (m_layout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (m_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else if (m_layout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destination_stage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	}
	else if (m_layout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destination_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	}
	else
		ThrowException("unsupported layout transition!");

	command_list->PipelineBarrier(source_stage, destination_stage, barrier);

	m_layout = newLayout;
}

VkImage VkImageWrapper::GetImage() const
{
	return m_image;
}

VkDeviceMemory VkImageWrapper::GetImageMemory() const
{
	return m_image_memory;
}
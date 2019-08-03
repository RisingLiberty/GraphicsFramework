#include "stdafx.h"

#include "VkImageViewWrapper.h"
#include "VkHelperMethods.h"

VkImageViewWrapper::VkImageViewWrapper(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
{
	VkImageViewCreateInfo view_info = {};
	view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	view_info.image = image;
	view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	view_info.format = format;

	view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	view_info.subresourceRange.aspectMask = aspectFlags;
	view_info.subresourceRange.baseMipLevel = 0;
	view_info.subresourceRange.levelCount = mipLevels;
	view_info.subresourceRange.baseArrayLayer = 0;
	view_info.subresourceRange.levelCount = 1;

	VKCALL(vkCreateImageView(GetVkDevice(), &view_info, GetVkAllocationCallbacks(), &m_image_view));
}

VkImageViewWrapper::~VkImageViewWrapper()
{
	vkDestroyImageView(GetVkDevice(), m_image_view, GetVkAllocationCallbacks());
}

VkImageView VkImageViewWrapper::GetApiImageView() const
{
	return m_image_view;
}
#pragma once

class VkImageViewWrapper
{
public:
	VkImageViewWrapper(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	~VkImageViewWrapper();

	VkImageView GetApiImageView() const;

private:
	VkImageView m_image_view;
};
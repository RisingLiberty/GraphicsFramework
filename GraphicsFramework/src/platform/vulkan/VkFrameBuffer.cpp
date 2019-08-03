#include "stdafx.h"

#include "VkFrameBuffer.h"
#include "VkHelperMethods.h"

VkFrameBufferWrapper::VkFrameBufferWrapper(unsigned int width, unsigned int height, const std::vector<VkImageView>& attachments, VkRenderPass renderPass)
{
	VkFramebufferCreateInfo framebuffer_info = {};
	framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebuffer_info.renderPass = renderPass;
	framebuffer_info.attachmentCount = static_cast<uint32_t>(attachments.size());
	framebuffer_info.pAttachments = attachments.data();

	framebuffer_info.width = width;
	framebuffer_info.height = height;
	framebuffer_info.layers = 1;

	VKCALL(vkCreateFramebuffer(GetVkDevice(), &framebuffer_info, nullptr, &m_frame_buffer));
}

VkFrameBufferWrapper::~VkFrameBufferWrapper()
{
	vkDestroyFramebuffer(GetVkDevice(), m_frame_buffer, nullptr);
}

VkFramebuffer VkFrameBufferWrapper::GetApiFrameBuffer() const
{
	return m_frame_buffer;
}
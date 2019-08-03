#pragma once

class VkFrameBufferWrapper
{
public:
	VkFrameBufferWrapper(unsigned int width, unsigned int height, const std::vector<VkImageView>& attachments, VkRenderPass renderPass);
	~VkFrameBufferWrapper();

	VkFramebuffer GetApiFrameBuffer() const;

private:
	VkFramebuffer m_frame_buffer;
};
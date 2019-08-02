#pragma once

class VkCommand;

#include "graphics/GraphicsResource.h"

class VkCommandList : public GraphicsResource<VkCommandList>
{
public:
	VkCommandList();
	VkCommandList(unsigned int familyIndex);
	virtual ~VkCommandList();

	static void CreatePool(unsigned int familyIndex);

	void Begin(VkCommandBufferBeginInfo beginInfo);
	void End();
	void BeginRenderPass(VkRenderPassBeginInfo renderPassBeginInfo);
	void EndRenderPass();

	void BindDescriptorSet(VkPipelineLayout pipelineLayout, VkDescriptorSet descriptorSet);
	void BindPipeline(VkPipeline pipeline);
	void BindIndexBuffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType);
	void BindVertexBuffer(VkBuffer buffer, VkDeviceSize offset);
	void PipelineBarrier(VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage, VkImageMemoryBarrier barrier);

	VkResult AcquireNextImage(VkSwapchainKHR swapchain, unsigned int& imageIndex) const;

	VkCommandBuffer GetApiBuffer() const;

	VkSubmitInfo GetDirectSubmitInfo() const;
	VkSubmitInfo* GetSubmitInfo() const;

	VkSemaphore GetImageAvailableSemaphore() const;
	VkSemaphore GetRenderFinishedSemaphore() const;

private:
	VkCommandBuffer m_buffer;
	static VkCommandPool m_pool;
	
	VkSemaphore m_image_available_semaphore;
	VkSemaphore m_render_finished_semaphore;

	mutable VkPipelineStageFlags m_submit_wait_stages;

	std::vector<std::unique_ptr<VkCommand>> m_commands;
};
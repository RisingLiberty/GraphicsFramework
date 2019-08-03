#pragma once

class VkCommand;
class VkCommandPoolWrapper;
class VkSwapchain;

#include "graphics/GraphicsResource.h"

class VkCommandList : public GraphicsResource<VkCommandList>
{
public:
	VkCommandList(VkCommandPoolWrapper* commandPool = nullptr);
	VkCommandList(unsigned int familyIndex, VkCommandPoolWrapper* commandPool = nullptr);
	virtual ~VkCommandList();

	void Begin(VkCommandBufferBeginInfo beginInfo);
	void End();
	void BeginRenderPass(VkRenderPassBeginInfo renderPassBeginInfo);
	void EndRenderPass();

	void BindDescriptorSet(VkPipelineLayout pipelineLayout, VkDescriptorSet descriptorSet);
	void BindPipeline(VkPipeline pipeline);
	void BindIndexBuffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType);
	void BindVertexBuffer(VkBuffer buffer, VkDeviceSize offset);
	void PipelineBarrier(VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage, VkImageMemoryBarrier barrier);

	static void SetGlobalCommandPool(VkCommandPool pool);

	VkResult AcquireNextImage(VkSwapchain* swapchain, unsigned int& imageIndex) const;

	VkCommandBuffer GetApiBuffer() const;

	VkSubmitInfo GetDirectSubmitInfo() const;
	VkSubmitInfo GetSubmitInfo() const;

	VkSemaphore GetImageAvailableSemaphore() const;
	VkSemaphore GetRenderFinishedSemaphore() const;

protected:
	VkCommandBuffer m_buffer;
	static VkCommandPool s_pool;

	VkCommandPool m_pool;
	VkSemaphore m_image_available_semaphore;
	VkSemaphore m_render_finished_semaphore;

	std::vector<std::unique_ptr<VkCommand>> m_commands;
};
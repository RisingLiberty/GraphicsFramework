#include "stdafx.h"

#include "VkCommandList.h"

#include "VkHelperMethods.h"
#include "VkCommand.h"
#include "VkSwapchain.h"

VkCommandPool VkCommandList::m_pool = VK_NULL_HANDLE;

VkCommandList::VkCommandList()
{
	assert(m_pool);

	VkCommandBufferAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_info.commandPool = m_pool;
	alloc_info.commandBufferCount = 1;

	VKCALL(vkAllocateCommandBuffers(GetVkDevice(), &alloc_info, &m_buffer));

	VkSemaphoreCreateInfo semaphore_info = {};
	semaphore_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

	VKCALL(vkCreateSemaphore(GetVkDevice(), &semaphore_info, nullptr, &m_image_available_semaphore));
	VKCALL(vkCreateSemaphore(GetVkDevice(), &semaphore_info, nullptr, &m_render_finished_semaphore));
}

VkCommandList::VkCommandList(unsigned int familyIndex)
{
	assert(m_pool);

	VkCommandBufferAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_info.commandPool = m_pool;
	alloc_info.commandBufferCount = 1;

	VKCALL(vkAllocateCommandBuffers(GetVkDevice(), &alloc_info, &m_buffer));

	VkSemaphoreCreateInfo semaphore_info = {};
	semaphore_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

	VKCALL(vkCreateSemaphore(GetVkDevice(), &semaphore_info, nullptr, &m_image_available_semaphore));
	VKCALL(vkCreateSemaphore(GetVkDevice(), &semaphore_info, nullptr, &m_render_finished_semaphore));

}

VkCommandList::~VkCommandList()
{
	vkFreeCommandBuffers(GetVkDevice(), m_pool, 1, &m_buffer);

	if (this->GetCount() == 1)
		vkDestroyCommandPool(GetVkDevice(), m_pool, nullptr);
}

void VkCommandList::CreatePool(unsigned int familyIndex)
{
	VkCommandPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.queueFamilyIndex = familyIndex;
	pool_info.flags = 0;

	VKCALL(vkCreateCommandPool(GetVkDevice(), &pool_info, nullptr, &m_pool));
}

void VkCommandList::Begin(VkCommandBufferBeginInfo beginInfo)
{
	VKCALL(vkBeginCommandBuffer(m_buffer, &beginInfo));
}

void VkCommandList::End()
{
	VKCALL(vkEndCommandBuffer(m_buffer));
}

void VkCommandList::BeginRenderPass(VkRenderPassBeginInfo renderPassBeginInfo)
{
	vkCmdBeginRenderPass(m_buffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VkCommandList::EndRenderPass()
{
	vkCmdEndRenderPass(m_buffer);
}

void VkCommandList::BindDescriptorSet(VkPipelineLayout pipelineLayout, VkDescriptorSet descriptorSet)
{
	vkCmdBindDescriptorSets(m_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
}

void VkCommandList::BindPipeline(VkPipeline pipeline)
{
	vkCmdBindPipeline(m_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}

void VkCommandList::BindIndexBuffer(VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType)
{
	vkCmdBindIndexBuffer(m_buffer, buffer, offset, indexType);
}

void VkCommandList::BindVertexBuffer(VkBuffer buffer, VkDeviceSize offset)
{
	vkCmdBindVertexBuffers(m_buffer, 0, 1, &buffer, &offset);
}

void VkCommandList::PipelineBarrier(VkPipelineStageFlags srcStage, VkPipelineStageFlags dstStage, VkImageMemoryBarrier barrier)
{
	vkCmdPipelineBarrier(m_buffer,
		srcStage, dstStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier);
}

VkResult VkCommandList::AcquireNextImage(VkSwapchain* swapchain, unsigned int& imageIndex) const
{
	return vkAcquireNextImageKHR(GetVkDevice(), swapchain->GetSwapchain(), std::numeric_limits<uint64_t>::max(), m_image_available_semaphore, VK_NULL_HANDLE, &imageIndex);
}

VkCommandBuffer VkCommandList::GetApiBuffer() const
{
	return m_buffer;
}

VkSubmitInfo VkCommandList::GetDirectSubmitInfo() const
{
	VkSubmitInfo submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &m_buffer;

	return submit_info;
}

VkSubmitInfo* VkCommandList::GetSubmitInfo() const
{
	VkSubmitInfo* submit_info = new VkSubmitInfo();
	submit_info->sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	m_submit_wait_stages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	submit_info->waitSemaphoreCount = 1;
	submit_info->pWaitSemaphores = &m_image_available_semaphore;
	submit_info->pWaitDstStageMask = &m_submit_wait_stages;
	submit_info->commandBufferCount = 1;
	submit_info->pCommandBuffers = &m_buffer;

	submit_info->signalSemaphoreCount = 1;
	submit_info->pSignalSemaphores = &m_render_finished_semaphore;

	return submit_info;
}

VkSemaphore VkCommandList::GetImageAvailableSemaphore() const
{
	return m_image_available_semaphore;
}

VkSemaphore VkCommandList::GetRenderFinishedSemaphore() const
{
	return m_render_finished_semaphore;
}
#include "stdafx.h"

#include "VkCommandQueue.h"
#include "VkCommandList.h"
#include "VkHelperMethods.h"

VkCommandQueue::VkCommandQueue(unsigned int familyIndex, unsigned int maxFramesInFlight)
{
	vkGetDeviceQueue(GetVkDevice(), familyIndex, 0, &m_queue);

	VkSemaphoreCreateInfo semaphore_create_info = {};
	semaphore_create_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

	VkFenceCreateInfo fence_create_info = {};
	fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	m_in_flight_fences.resize(maxFramesInFlight);

	for (unsigned int i = 0; i < maxFramesInFlight; ++i)
	{
		VKCALL(vkCreateFence(GetVkDevice(), &fence_create_info, nullptr, &m_in_flight_fences[i]));
		this->Push(std::make_unique<VkCommandList>(familyIndex));
	}
}

VkCommandQueue::~VkCommandQueue()
{
	for (size_t i = 0; i < m_in_flight_fences.size(); ++i)
	{
		vkDestroyFence(GetVkDevice(), m_in_flight_fences[i], nullptr);
	}
}

void VkCommandQueue::Push(std::unique_ptr<VkCommandList> commandQueue)
{
	m_command_lists.push_back(std::move(commandQueue));
}

void VkCommandQueue::WaitForFence(unsigned int currentFrame)
{
	vkWaitForFences(GetVkDevice(), 1, &m_in_flight_fences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
}

void VkCommandQueue::DirectSubmit(VkSubmitInfo submitInfo)
{
	VKCALL(vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE));
	vkQueueWaitIdle(m_queue);
}

void VkCommandQueue::Submit(VkCommandList* commandList, unsigned int currentFrame)
{
	VkSemaphore image_available_semaphore = commandList->GetImageAvailableSemaphore();
	VkSemaphore render_finished_semaphore = commandList->GetRenderFinishedSemaphore();
	VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	VkSubmitInfo submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.waitSemaphoreCount = 1;
	submit_info.pWaitSemaphores = &image_available_semaphore;
	submit_info.pWaitDstStageMask = wait_stages;
	submit_info.commandBufferCount = 1;

	VkCommandBuffer cmd_buffer = commandList->GetApiBuffer();
	submit_info.pCommandBuffers = &cmd_buffer;
	submit_info.signalSemaphoreCount = 1;
	submit_info.pSignalSemaphores = &render_finished_semaphore;

	vkResetFences(GetVkDevice(), 1, &m_in_flight_fences[currentFrame]);
	VKCALL(vkQueueSubmit(m_queue, 1, &submit_info, m_in_flight_fences[currentFrame]));
}

void VkCommandQueue::Submit(VkSemaphore imageAvailableSemaphore, VkSemaphore renderFinishedSemaphore, VkCommandBuffer buffer, unsigned int currentFrame) const
{
	VkSubmitInfo submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	submit_info.waitSemaphoreCount = 1;
	submit_info.pWaitSemaphores = &imageAvailableSemaphore;
	submit_info.pWaitDstStageMask = wait_stages;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &buffer;
			   
	submit_info.signalSemaphoreCount = 1;
	submit_info.pSignalSemaphores = &renderFinishedSemaphore;

	vkResetFences(GetVkDevice(), 1, &m_in_flight_fences[currentFrame]);
	VKCALL(vkQueueSubmit(m_queue, 1, &submit_info, m_in_flight_fences[currentFrame]));
}

void VkCommandQueue::Present(VkSwapchainKHR swapchain, VkSemaphore renderFinishedSemaphore, unsigned int currentFrame) const
{
	VkPresentInfoKHR present_info = {};
	present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present_info.waitSemaphoreCount = 1;
	present_info.pWaitSemaphores = &renderFinishedSemaphore;
	present_info.swapchainCount = 1;
	present_info.pSwapchains = &swapchain;
	present_info.pImageIndices = &currentFrame;
	present_info.pResults = nullptr;

	VKCALL(vkQueuePresentKHR(m_queue, &present_info));

	vkQueueWaitIdle(m_queue);
}

VkQueue VkCommandQueue::GetApiQueue() const
{
	return m_queue;
}

VkCommandList* VkCommandQueue::GetApiList(unsigned int currentFrame) const
{
	return m_command_lists[currentFrame].get();
}

VkFence VkCommandQueue::GetCurrentInFlightFence(unsigned int currentFrame) const
{
	return m_in_flight_fences[currentFrame];
}
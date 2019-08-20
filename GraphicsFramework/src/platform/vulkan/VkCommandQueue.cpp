#include "stdafx.h"

#include "VkCommandQueue.h"
#include "VkCommandList.h"
#include "VkHelperMethods.h"
#include "VkCommandPoolWrapper.h"
#include "VkDirectCommandList.h"

VkCommandQueue::VkCommandQueue(unsigned int familyIndex, unsigned int maxFramesInFlight):
	CommandQueue(maxFramesInFlight)
{
	vkGetDeviceQueue(GetVkDevice(), familyIndex, 0, &m_queue);
	m_command_pool = std::make_unique<VkCommandPoolWrapper>(familyIndex);
	VkCommandList::SetGlobalCommandPool(m_command_pool->GetApiCommandPool());

	VkSemaphoreCreateInfo semaphore_create_info = {};
	semaphore_create_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

	VkFenceCreateInfo fence_create_info = {};
	fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	m_in_flight_fences.resize(maxFramesInFlight);

	for (unsigned int i = 0; i < maxFramesInFlight; ++i)
	{
		VKCALL(vkCreateFence(GetVkDevice(), &fence_create_info, GetVkAllocationCallbacks(), &m_in_flight_fences[i]));
		m_command_lists[i] = std::make_unique<VkCommandList>(familyIndex, m_command_pool.get());
	}
}

VkCommandQueue::~VkCommandQueue()
{
	for (size_t i = 0; i < m_in_flight_fences.size(); ++i)
	{
		vkDestroyFence(GetVkDevice(), m_in_flight_fences[i], GetVkAllocationCallbacks());
	}
}

void VkCommandQueue::WaitForFence(unsigned int currentFrame)
{
	vkWaitForFences(GetVkDevice(), 1, &m_in_flight_fences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
}

void VkCommandQueue::DirectSubmit(VkSubmitInfo submitInfo) const
{
	VKCALL(vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE));
	vkQueueWaitIdle(m_queue);
}

void VkCommandQueue::Submit(VkCommandList* commandList, unsigned int currentFrame)
{
	VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	VkSubmitInfo submit_info = commandList->GetSubmitInfo();
	submit_info.pWaitDstStageMask = wait_stages;

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

void VkCommandQueue::WaitTillIdle()
{
	vkQueueWaitIdle(m_queue);
}

VkQueue VkCommandQueue::GetApiQueue() const
{
	return m_queue;
}

VkCommandList* VkCommandQueue::GetApiList(unsigned int currentFrame) const
{
	return m_command_lists[currentFrame]->As<VkCommandList>();
}

VkFence VkCommandQueue::GetCurrentInFlightFence(unsigned int currentFrame) const
{
	return m_in_flight_fences[currentFrame];
}

std::unique_ptr<VkCommandList> VkCommandQueue::CreateDirectCommandList() const
{
	return std::make_unique<VkDirectCommandList>(this);
}
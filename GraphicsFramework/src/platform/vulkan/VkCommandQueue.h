#pragma once

#include "graphics/CommandQueue.h"

class VkCommandPoolWrapper;
class VkCommandList;

class VkCommandQueue : public CommandQueue
{
public:
	VkCommandQueue(unsigned int familyIndex, unsigned int maxFramesInFlight);
	virtual ~VkCommandQueue();

	void WaitForFence(unsigned int currentFrame);

	void Submit(VkCommandList* commandList, unsigned int currentFrame);
	void DirectSubmit(VkSubmitInfo submitInfo) const;
	void Present(VkSwapchainKHR swapchain, VkSemaphore renderFinishedSemaphore, unsigned int currentFrame) const;
	void WaitTillIdle();

	VkQueue GetApiQueue() const;
	VkCommandList* GetApiList(unsigned int currentFrame) const;
	VkFence GetCurrentInFlightFence(unsigned int currentFence) const;
	std::unique_ptr<VkCommandList> CreateDirectCommandList() const;

private:
	VkQueue m_queue;
	std::unique_ptr<VkCommandPoolWrapper> m_command_pool;

	std::vector<VkFence> m_in_flight_fences;
};
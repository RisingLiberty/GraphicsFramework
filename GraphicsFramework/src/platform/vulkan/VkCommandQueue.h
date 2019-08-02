#pragma once

class VkCommandList;

class VkCommandQueue
{
public:
	VkCommandQueue(unsigned int familyIndex, unsigned int maxFramesInFlight);
	virtual ~VkCommandQueue();

	void Push(std::unique_ptr<VkCommandList> commandList);
	void WaitForFence(unsigned int currentFrame);

	void Submit(VkCommandList* commandList, unsigned int currentFrame);
	void Submit(VkSemaphore imageAvailableSemaphore, VkSemaphore renderFinishedSemaphore, VkCommandBuffer buffer, unsigned int currentFrame) const;
	void DirectSubmit(VkSubmitInfo submitInfo);
	void Present(VkSwapchainKHR swapchain, VkSemaphore renderFinishedSemaphore, unsigned int currentFrame) const;

	VkQueue GetApiQueue() const;
	VkCommandList* GetApiList(unsigned int currentFrame) const;
	VkFence GetCurrentInFlightFence(unsigned int currentFence) const;
private:
	VkQueue m_queue;
	std::vector<std::unique_ptr<VkCommandList>> m_command_lists;

	std::vector<VkFence> m_in_flight_fences;
};
#pragma once

class VkCommandPoolWrapper;
class VkCommandList;

class VkCommandQueue
{
public:
	VkCommandQueue(unsigned int familyIndex, unsigned int maxFramesInFlight);
	virtual ~VkCommandQueue();

	void Push(std::unique_ptr<VkCommandList> commandList);
	void WaitForFence(unsigned int currentFrame);

	void Submit(VkCommandList* commandList, unsigned int currentFrame);
	void DirectSubmit(VkSubmitInfo submitInfo) const;
	void Present(VkSwapchainKHR swapchain, VkSemaphore renderFinishedSemaphore, unsigned int currentFrame) const;

	VkQueue GetApiQueue() const;
	VkCommandList* GetApiList(unsigned int currentFrame) const;
	VkFence GetCurrentInFlightFence(unsigned int currentFence) const;
	std::unique_ptr<VkCommandList> CreateDirectCommandList() const;

private:
	VkQueue m_queue;
	std::unique_ptr<VkCommandPoolWrapper> m_command_pool;
	std::vector<std::unique_ptr<VkCommandList>> m_command_lists;

	std::vector<VkFence> m_in_flight_fences;
};
#pragma once

#include "VkSwapchainSupportDetails.h"

class VkSwapchain
{
public:
	VkSwapchain(const SwapChainSupportDetails& details, VkFormat format, VkSurfaceKHR surface, unsigned int graphicsPresentQueueIndex);
	~VkSwapchain();
	
	VkSwapchainKHR GetSwapchain() const;
	VkExtent2D GetExtent() const;
	std::vector<VkImage> GetImages() const;

private:
	VkSwapchainKHR m_swapchain;
	VkExtent2D m_extent;
	unsigned int m_image_count;
};
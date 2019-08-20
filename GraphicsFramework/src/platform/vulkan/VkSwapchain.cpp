#include "stdafx.h"

#include "VkSwapchain.h"
#include "VkHelperMethods.h"
#include "VkCommandList.h"

namespace
{
	VkSurfaceFormatKHR ChooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
			return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

		for (const VkSurfaceFormatKHR& available_format : availableFormats)
		{
			if (available_format.format == VK_FORMAT_B8G8R8A8_UNORM && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return available_format;
		}

		return availableFormats[0];
	}
	VkPresentModeKHR ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
	{
		VkPresentModeKHR best_mode = VK_PRESENT_MODE_FIFO_KHR;

		for (const VkPresentModeKHR& available_present_mode : availablePresentModes)
		{
			if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
				return available_present_mode;
			else if (available_present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
				best_mode = available_present_mode;
		}

		return best_mode;
	}

	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
			return capabilities.currentExtent;

		else
			ASSERT(false, "current extent is invalid!");

		return {};
	}

}

VkSwapchain::VkSwapchain(const SwapChainSupportDetails& details, VkFormat format, VkSurfaceKHR surface, unsigned int graphicsPresentQueueIndex)
{
	m_image_count = details.capabilities.minImageCount + 1;

	VkSurfaceFormatKHR surface_format = ChooseSwapchainSurfaceFormat(details.formats);

	spdlog::warn("Changing back buffer format to self given value!");
	surface_format.format = format;

	VkPresentModeKHR present_mode = ChooseSwapchainPresentMode(details.present_modes);
	VkExtent2D extent = ChooseSwapExtent(details.capabilities);

	uint32_t image_count = details.capabilities.minImageCount + 1;
	if (details.capabilities.minImageCount > 0 && image_count > details.capabilities.maxImageCount)
		image_count = details.capabilities.maxImageCount;

	VkSwapchainCreateInfoKHR create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	create_info.surface = surface;
	create_info.minImageCount = image_count;
	create_info.minImageCount = image_count;
	create_info.imageFormat = surface_format.format;
	create_info.imageColorSpace = surface_format.colorSpace;
	create_info.imageExtent = extent;
	create_info.imageArrayLayers = 1;
	create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	create_info.queueFamilyIndexCount = 0;
	create_info.pQueueFamilyIndices = nullptr;

	create_info.preTransform = details.capabilities.currentTransform;
	create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	create_info.presentMode = present_mode;
	create_info.clipped = VK_TRUE;

	create_info.oldSwapchain = VK_NULL_HANDLE;

	VkResult result = vkCreateSwapchainKHR(GetVkDevice(), &create_info, nullptr, &m_swapchain);
	//VKCALL(vkCreateSwapchainKHR(GetVkDevice(), &create_info, nullptr, &m_swapchain));

	m_extent = extent;
}

VkSwapchain::~VkSwapchain()
{
	vkDestroySwapchainKHR(GetVkDevice(), m_swapchain, GetVkAllocationCallbacks());
}

VkSwapchainKHR VkSwapchain::GetSwapchain() const
{
	return m_swapchain;
}

VkExtent2D VkSwapchain::GetExtent() const
{
	return m_extent;
}

std::vector<VkImage> VkSwapchain::GetImages() const
{
	unsigned int image_count = m_image_count;
	std::vector<VkImage> images(image_count);
	vkGetSwapchainImagesKHR(GetVkDevice(), m_swapchain, &image_count, images.data());

	return images;

}
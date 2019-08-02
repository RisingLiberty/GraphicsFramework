#pragma once

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> present_modes;
};

class VkGpu
{
public:
	VkGpu(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	~VkGpu();

	bool SupportExtensions() const;

	VkPhysicalDevice GetPhysicalDevice() const;
	SwapChainSupportDetails GetSwapchainSupportDetails() const;
	unsigned int GetDeviceScore() const;
	unsigned int GetGraphicsPresentQueueIndex() const;
	bool IsSuitable() const;

private:
	unsigned int FindGraphicsAndPresentQueueFamilyIndex(VkSurfaceKHR surface) const;
	unsigned int CalculateDeviceScore() const;
	SwapChainSupportDetails QuerySwapchainSupport(VkSurfaceKHR surface) const;
	VkSampleCountFlagBits GetMaxUsableSampleCount() const;
	bool IsSuitable(VkSurfaceKHR surface) const;

private:
	VkPhysicalDevice m_physical_device;
	VkSampleCountFlagBits m_msaa_samples;
	const std::vector<const char*> m_device_extentions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	SwapChainSupportDetails m_swapchain_support_details;

	unsigned int m_graphics_present_queue_index;
	unsigned int m_score;
	bool m_is_suitable;

};
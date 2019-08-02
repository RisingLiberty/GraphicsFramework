#include "stdafx.h"

#include "VkGpu.h"

VkGpu::VkGpu(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface):
	m_physical_device(physicalDevice)
{
	m_msaa_samples = GetMaxUsableSampleCount();
	m_graphics_present_queue_index = this->FindGraphicsAndPresentQueueFamilyIndex(surface);
	m_is_suitable = this->IsSuitable(surface);
	m_score = this->CalculateDeviceScore();
	m_swapchain_support_details = this->QuerySwapchainSupport(surface);
}

VkGpu::~VkGpu()
{
}

bool VkGpu::IsSuitable(VkSurfaceKHR surface) const
{
	bool extension_support = this->SupportExtensions();
	bool swapchain_adequate = false;

	if (extension_support)
	{
		SwapChainSupportDetails swapchain_support = QuerySwapchainSupport(surface);
		swapchain_adequate = !swapchain_support.formats.empty() && !swapchain_support.present_modes.empty();
	}

	VkPhysicalDeviceFeatures supported_features;
	vkGetPhysicalDeviceFeatures(m_physical_device, &supported_features);

	return extension_support && swapchain_adequate && supported_features.samplerAnisotropy;
}

unsigned int VkGpu::FindGraphicsAndPresentQueueFamilyIndex(VkSurfaceKHR surface) const
{
	uint32_t queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &queue_family_count, nullptr);

	std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &queue_family_count, queue_families.data());

	unsigned int family_index = -1;
	unsigned int present_index = -1;
	int i = 0;
	for (const VkQueueFamilyProperties& queue_family : queue_families)
	{
		// supports graphics
		if (queue_family.queueCount > 0 && queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			family_index = i;

		// supports swapchain presenting
		VkBool32 present_support = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(m_physical_device, i, surface, &present_support);
		if (queue_family.queueCount >= 0 && present_support)
			present_index = i;

		if (family_index != -1 && present_index != -1)
			break;

		++i;
	}
	
	ASSERT(present_index == family_index, "Present Queue != Graphics Queue!");
	return family_index;
}

unsigned int VkGpu::CalculateDeviceScore() const
{
	VkPhysicalDeviceProperties gpu_properties;
	vkGetPhysicalDeviceProperties(m_physical_device, &gpu_properties);

	VkPhysicalDeviceFeatures gpu_features;
	vkGetPhysicalDeviceFeatures(m_physical_device, &gpu_features);

	//return gpu_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && gpu_features.geometryShader;

	int score = 0;

	if (gpu_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		score += 1000;

	score += gpu_properties.limits.maxImageDimension2D;

	if (!gpu_features.geometryShader)
		return 0;

	return score;
}

SwapChainSupportDetails VkGpu::QuerySwapchainSupport(VkSurfaceKHR surface) const
{
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physical_device, surface, &details.capabilities);

	uint32_t format_count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(m_physical_device, surface, &format_count, nullptr);

	if (format_count != 0)
	{
		details.formats.resize(format_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_physical_device, surface, &format_count, details.formats.data());
	}

	uint32_t present_mode_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(m_physical_device, surface, &present_mode_count, nullptr);

	if (present_mode_count != 0)
	{
		details.present_modes.resize(present_mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_physical_device, surface, &present_mode_count, details.present_modes.data());
	}

	return details;
}

bool VkGpu::SupportExtensions() const
{
	uint32_t extension_count;
	vkEnumerateDeviceExtensionProperties(m_physical_device, nullptr, &extension_count, nullptr);
	std::vector<VkExtensionProperties> available_extensions(extension_count);

	vkEnumerateDeviceExtensionProperties(m_physical_device, nullptr, &extension_count, available_extensions.data());

	std::set<std::string> required_extensions(m_device_extentions.begin(), m_device_extentions.end());

	for (const VkExtensionProperties& extension : available_extensions)
		required_extensions.erase(extension.extensionName);

	return required_extensions.empty();
}

VkPhysicalDevice VkGpu::GetPhysicalDevice() const
{
	return m_physical_device;
}

SwapChainSupportDetails VkGpu::GetSwapchainSupportDetails() const
{
	return m_swapchain_support_details;
}

VkSampleCountFlagBits VkGpu::GetMaxUsableSampleCount() const
{
	return m_msaa_samples;
}

unsigned int VkGpu::GetDeviceScore() const
{
	return m_score;
}

unsigned int VkGpu::GetGraphicsPresentQueueIndex() const
{
	return m_graphics_present_queue_index;
}

bool VkGpu::IsSuitable() const
{
	return m_is_suitable;
}
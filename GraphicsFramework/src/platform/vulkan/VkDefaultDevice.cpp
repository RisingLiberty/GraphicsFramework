#include "stdafx.h"

#include "VkDefaultDevice.h"
#include "VkHelperMethods.h"
#include "VkGpu.h"

VkDefaultDevice::VkDefaultDevice(VkGpu* gpu)
{
	float queue_priority = 1.0f;

	VkDeviceQueueCreateInfo queue_create_info = {};
	queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info.queueFamilyIndex = gpu->GetGraphicsPresentQueueIndex();
	queue_create_info.queueCount = 1;
	queue_create_info.pQueuePriorities = &queue_priority;

	VkDeviceCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	create_info.pQueueCreateInfos = &queue_create_info;
	create_info.queueCreateInfoCount = 1;

	VkPhysicalDeviceFeatures device_features;
	vkGetPhysicalDeviceFeatures(gpu->GetPhysicalDevice(), &device_features);
	device_features.samplerAnisotropy = VK_TRUE;
	device_features.sampleRateShading = VK_TRUE;

	create_info.pEnabledFeatures = &device_features;

	create_info.enabledExtensionCount = static_cast<uint32_t>(m_extentions.size());
	create_info.ppEnabledExtensionNames = m_extentions.data();

	create_info.enabledLayerCount = static_cast<uint32_t>(m_validation_layers.size());
	create_info.ppEnabledLayerNames = m_validation_layers.data();


	VKCALL(vkCreateDevice(gpu->GetPhysicalDevice(), &create_info, GetVkAllocationCallbacks(), &m_device));
}

VkDefaultDevice::~VkDefaultDevice()
{
	vkDestroyDevice(m_device, GetVkAllocationCallbacks());
}

VkDevice VkDefaultDevice::GetApiDevice() const
{
	return m_device;
}
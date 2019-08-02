#pragma once

class VkGpu;

class VkDefaultDevice
{
public:
	VkDefaultDevice(VkGpu* gpu);
	~VkDefaultDevice();

	VkDevice GetApiDevice() const;

private:
	VkDevice m_device;

	const std::vector<const char*> m_validation_layers = { "VK_LAYER_LUNARG_standard_validation" };
	const std::vector<const char*> m_extentions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
};
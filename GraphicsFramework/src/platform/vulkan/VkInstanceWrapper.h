#pragma once

class Window;

class VkInstanceWrapper
{
public:
	VkInstanceWrapper(Window* window);
	~VkInstanceWrapper();

	VkSurfaceKHR CreateSurface(Window* window);

	VkInstance GetApiInstance() const;
	std::vector<VkExtensionProperties> GetExtensions() const;
private:
	void SetupDebugCallback();

private:
	VkInstance m_instance;
	VkDebugUtilsMessengerEXT m_debug_callback;

};
#include "stdafx.h"

#include "VkInstanceWrapper.h"
#include "VkHelperMethods.h"
#include "graphics/Window.h"

namespace
{
	const std::vector<const char*> VALIDATION_LAYERS = { "VK_LAYER_LUNARG_standard_validation" };


	bool CheckValidationLayerSupport()
	{
		uint32_t layer_count = 0;
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

		std::vector<VkLayerProperties> availableLayers(layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, availableLayers.data());

		for (const char* layerName : VALIDATION_LAYERS)
		{
			bool layer_found = false;

			for (const VkLayerProperties& layer_props : availableLayers)
			{
				if (strcmp(layerName, layer_props.layerName) == 0)
				{
					layer_found = true;
					break;
				}
			}

			if (!layer_found)
				return false;
		}

		return true;
	}

	std::vector<const char*> GetRequiredExtentions()
	{
		// these are normally the extentsions returned by glfw
		// if surface is enabled (which is true in this case)
		// and we're developing for windows

		return
		{
			_strdup("VK_KHR_surface"),
			_strdup("VK_KHR_win32_surface"),
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME
		};

		//TODO: get real extentions from glfw
		//uint32_t glfwExtensionCount = 0;
		//const char** glfwExtensions;
		//glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		//std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		//if (m_EnableValidationLayers)
		//	extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		//return extensions;
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
			func(instance, callback, pAllocator);
	}

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

		if (func != nullptr)
			return func(instance, pCreateInfo, pAllocator, pCallback);
		else
			return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	typedef VkFlags VkWin32SurfaceCreateFlagsKHR;
	typedef struct VkWin32SurfaceCreateInfoKHR
	{
		VkStructureType                 sType;
		const void*                     pNext;
		VkWin32SurfaceCreateFlagsKHR    flags;
		HINSTANCE                       hinstance;
		HWND                            hwnd;
	} VkWin32SurfaceCreateInfoKHR;
	typedef VkResult(APIENTRY *PFN_vkCreateWin32SurfaceKHR)(VkInstance, const VkWin32SurfaceCreateInfoKHR*, const VkAllocationCallbacks*, VkSurfaceKHR*);

	VkResult CreateWindowSurface(VkInstance instance,
		Window* window,
		const VkAllocationCallbacks* allocator,
		VkSurfaceKHR* surface)
	{
		VkResult err;
		VkWin32SurfaceCreateInfoKHR surface_create_info;
		PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;

		vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)
			vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR");
		if (!vkCreateWin32SurfaceKHR)
		{
			spdlog::error("Win32: Vulkan instance missing VK_KHR_win32_surface extension");
			return VkResult::VK_ERROR_EXTENSION_NOT_PRESENT;
		}

		memset(&surface_create_info, 0, sizeof(surface_create_info));
		surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surface_create_info.hinstance = GetModuleHandle(NULL);
		surface_create_info.hwnd = (HWND)window->GetHandle();

		err = vkCreateWin32SurfaceKHR(instance, &surface_create_info, allocator, surface);
		if (err != VK_SUCCESS)
		{
			spdlog::error("Win32: Failed to create Vulkan surface");
		}

		return err;
	}

}

VkInstanceWrapper::VkInstanceWrapper(Window* window)
{
	if (!CheckValidationLayerSupport())
		ThrowException("validation layers requested, but not available!");

	VkApplicationInfo app_info = {};

	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = window->GetTitle().c_str();
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "No Engine";
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &app_info;

	std::vector<const char*> extentions = GetRequiredExtentions();
	create_info.enabledExtensionCount = static_cast<uint32_t>(extentions.size());
	create_info.ppEnabledExtensionNames = extentions.data();

	create_info.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
	create_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();

	VKCALL(vkCreateInstance(&create_info, GetVkAllocationCallbacks(), &m_instance));

	this->SetupDebugCallback();
}

VkInstanceWrapper::~VkInstanceWrapper()
{
	DestroyDebugUtilsMessengerEXT(m_instance, m_debug_callback, GetVkAllocationCallbacks());
	vkDestroyInstance(m_instance, GetVkAllocationCallbacks());
}

VkSurfaceKHR VkInstanceWrapper::CreateSurface(Window* window)
{
	VkSurfaceKHR surface;
	VKCALL(CreateWindowSurface(m_instance, window, GetVkAllocationCallbacks(), &surface));
	return surface;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	//Message is important enough to show
	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		std::cerr << "fatal error" << std::endl;

	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

void VkInstanceWrapper::SetupDebugCallback()
{
	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;/* |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;*/

	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	createInfo.pfnUserCallback = DebugCallback;
	createInfo.pUserData = nullptr; //Optional

	VKCALL(CreateDebugUtilsMessengerEXT(m_instance, &createInfo, GetVkAllocationCallbacks(), &m_debug_callback));
}

VkInstance VkInstanceWrapper::GetApiInstance() const
{
	return m_instance;
}

std::vector<VkExtensionProperties> VkInstanceWrapper::GetExtensions() const
{
	uint32_t extension_count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

	std::vector<VkExtensionProperties> extensions(extension_count);
	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

	return extensions;
}
#include "stdafx.h"

#include "VkContext.h"
#include "VkHelperMethods.h"

#include "graphics/Window.h"

namespace
{
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

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
			func(instance, callback, pAllocator);
	}
}

VkContext::VkContext(Window* window)
{
	this->CreateInstance(window);

}

VkContext::~VkContext()
{

}

void VkContext::Initialize()
{
	this->ShowExtentions();
	this->SetupDebugCallback();
	this->PickPhysicalDevice();
	this->CreateLogicalDevice();
	this->CreateSwapchain();
	this->CreateImageViews();
	this->CreateRenderPass();
	this->CreateDescriptorSetLayout();
	this->CreateGraphicsPipeline();
	this->CreateCommandPool();
	this->CreateColorResources();
	this->CreateDepthResources();
	this->CreateFrameBuffers();
	this->CreateTextureImage();
	this->CreateTextureImageView();
	this->CreateTextureSampler();
	this->LoadModel();
	this->CreateVertexBuffer();
	this->CreateIndexBuffer();
	this->CreateUniformBuffer();
	this->CreateDescriptorPool();
	this->CreateDescriptorSets();
	this->CreateCommandBuffers();
	this->CreateSyncObjects();
}

void VkContext::Cleanup()
{
	//Clean vulkan
	this->CleanupSwapchain();

	vkDestroyImageView(m_device, m_color_image_view, nullptr);
	vkDestroyImage(m_device, m_color_image, nullptr);
	vkFreeMemory(m_device, m_color_image_memory, nullptr);

	vkDestroyImage(m_device, m_depth_image, nullptr);
	vkDestroyImageView(m_device, m_depth_image_view, nullptr);
	vkFreeMemory(m_device, m_depth_image_memory, nullptr);

	vkDestroySampler(m_device, m_texture_sampler, nullptr);

	vkDestroyImageView(m_device, m_texture_image_view, nullptr);

	vkDestroyImage(m_device, m_texture_image, nullptr);
	vkFreeMemory(m_device, m_texture_image_memory, nullptr);

	vkDestroyDescriptorPool(m_device, m_descriptor_pool, nullptr);

	vkDestroyDescriptorSetLayout(m_device, m_descriptor_set_layout, nullptr);

	for (size_t i = 0; i < m_swapchain_images.size(); ++i)
	{
		vkDestroyBuffer(m_device, m_uniform_buffers[i], nullptr);
		vkFreeMemory(m_device, m_uniform_buffers_memory[i], nullptr);
	}

	vkDestroyBuffer(m_device, m_index_buffer, nullptr);
	vkFreeMemory(m_device, m_index_buffer_memory, nullptr);

	vkDestroyBuffer(m_device, m_vertex_buffer, nullptr);
	vkFreeMemory(m_device, m_vertex_buffer_memory, nullptr);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		vkDestroySemaphore(m_device, m_render_finished_semaphores[i], nullptr);
		vkDestroySemaphore(m_device, m_image_available_semaphores[i], nullptr);
		vkDestroyFence(m_device, m_in_flight_fences[i], nullptr);
	}

	vkDestroyCommandPool(m_device, m_command_pool, nullptr);
	vkDestroyDevice(m_device, nullptr);

	if (m_enable_validation_layers)
		DestroyDebugUtilsMessengerEXT(m_instance, m_debug_callback, nullptr);

	vkDestroySurfaceKHR(m_instance, m_surface, nullptr);

	vkDestroyInstance(m_instance, nullptr);
}

void VkContext::CleanupSwapchain()
{
	for (size_t i = 0; i < m_swapchain_frame_buffers.size(); ++i)
		vkDestroyFramebuffer(m_device, m_swapchain_frame_buffers[i], nullptr);

	vkFreeCommandBuffers(m_device, m_command_pool, static_cast<uint32_t>(m_command_buffers.size()), m_command_buffers.data());
	vkDestroyPipeline(m_device, m_graphics_pipeline, nullptr);
	vkDestroyPipelineLayout(m_device, m_pipeline_layout, nullptr);
	vkDestroyRenderPass(m_device, m_render_pass, nullptr);

	for (size_t i = 0; i < m_swapchain_image_views.size(); ++i)
		vkDestroyImageView(m_device, m_swapchain_image_views[i], nullptr);

	vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
}

void VkContext::Present()
{
}

Context::API VkContext::GetApiType() const
{
	return API::VULKAN;
}

void VkContext::CreateInstance(Window* window)
{
	if (m_enable_validation_layers && !this->CheckValidationLayerSupport())
		ThrowException("vilidation layers requested, but not available!");

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
	
	if (m_enable_validation_layers)
	{
		create_info.enabledLayerCount = static_cast<uint32_t>(m_validation_layers.size());
		create_info.ppEnabledLayerNames = m_validation_layers.data();
	}
	else
		create_info.enabledExtensionCount = 0;

	VKCALL(vkCreateInstance(&create_info, nullptr, &m_instance));
}

void VkContext::ShowExtentions()
{
	uint32_t extension_count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

	std::vector<VkExtensionProperties> extensions(extension_count);
	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

	spdlog::info("available extensions:");

	for (const VkExtensionProperties& extension : extensions)
		spdlog::info("\t{}", extension.extensionName);
}

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
	VkDebugUtilsMessageTypeFlagsEXT messageType, 
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
	void* pUserData)
{
	if (messageSeverity > -VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		spdlog::error("Fatal Vulkan error!");

	spdlog::error("validation layer: {}", pCallbackData->pMessage);

	return VK_FALSE;
}

void VkContext::SetupDebugCallback()
{
	if (!m_enable_validation_layers)
		return;

	VkDebugUtilsMessengerCreateInfoEXT create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;

	create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	create_info.pfnUserCallback = DebugCallback;
	create_info.pUserData = nullptr; // Optional

	VKCALL(CreateDebugUtilsMessengerEXT(m_instance, &create_info, nullptr, &m_debug_callback));
}

void VkContext::PickPhysicalDevice()
{
}

void VkContext::CreateLogicalDevice()
{
}

void VkContext::CreateSwapchain()
{
}

void VkContext::CreateImageViews()
{
}

void VkContext::CreateRenderPass()
{
}

void VkContext::CreateDescriptorSetLayout()
{
}

void VkContext::CreateGraphicsPipeline()
{
}

void VkContext::CreateCommandPool()
{
}

void VkContext::CreateColorResources()
{
}

void VkContext::CreateDepthResources()
{
}

void VkContext::CreateFrameBuffers()
{
}

void VkContext::CreateTextureImage()
{
}

void VkContext::CreateTextureImageView()
{
}

void VkContext::CreateTextureSampler()
{
}

void VkContext::LoadModel()
{
}

void VkContext::CreateVertexBuffer()
{
}

void VkContext::CreateIndexBuffer()
{
}

void VkContext::CreateUniformBuffer()
{
}

void VkContext::CreateDescriptorPool()
{
}

void VkContext::CreateDescriptorSets()
{
}

void VkContext::CreateCommandBuffers()
{
}

void VkContext::CreateSyncObjects()
{
}

bool VkContext::CheckValidationLayerSupport() const
{
	uint32_t layer_count = 0;
	vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

	std::vector<VkLayerProperties> availableLayers(layer_count);
	vkEnumerateInstanceLayerProperties(&layer_count, availableLayers.data());

	for (const char* layerName : m_validation_layers)
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

std::vector<const char*> VkContext::GetRequiredExtentions() const
{
	// these are normally the extentsions returned by glfw
	// if surface is enabled (which is true in this case)
	// and we're developing for windows

	return 
	{
		strdup("VK_KHR_surface"),
		strdup("VK_KHR_win32_surface")
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
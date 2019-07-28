#include "stdafx.h"

#include "VkContext.h"
#include "VkHelperMethods.h"

#include "graphics/Window.h"

#include "VkVertexBuffer.h"
#include "VkIndexBuffer.h"

#include "VkVertexShader.h"
#include "VkFragmentShader.h"
#include "VkShaderProgram.h"
#include "VkVertexLayout.h"
#include "VkVertexArray.h"

#include "VkRenderer.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb/stb_image.h>

//Define to include the function bodies and avoid linker issues
//#ifndef TINYOBJLOADER_IMPLEMENTATION
//#define TINYOBJLOADER_IMPLEMENTATION
//#include <objreader/tiny_obj_loader.h>
//#endif


namespace vk_utils
{
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
		VkWin32SurfaceCreateInfoKHR sci;
		PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;

		vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)
			vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR");
		if (!vkCreateWin32SurfaceKHR)
		{
			spdlog::error("Win32: Vulkan instance missing VK_KHR_win32_surface extension");
			return VkResult::VK_ERROR_EXTENSION_NOT_PRESENT;
		}

		memset(&sci, 0, sizeof(sci));
		sci.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		sci.hinstance = GetModuleHandle(NULL);
		sci.hwnd = (HWND)window->GetHandle();

		err = vkCreateWin32SurfaceKHR(instance, &sci, allocator, surface);
		if (err != VK_SUCCESS)
		{
			spdlog::error("Win32: Failed to create Vulkan surface");
		}

		return err;
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

	bool HasStencilComponent(VkFormat format)
	{
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}
}

void CheckVkResult(VkResult err)
{
	VKCALL(err);
}

VkContext::VkContext(Window* window)
{
	spdlog::info("Using Vulkan");

	this->CreateInstance(window);
	this->ShowExtentions();
	this->CreateSurface(window);

	//this->CreateTextureImage();
	//this->CreateTextureImageView();
	//this->CreateTextureSampler();
	//this->LoadModel();
}

VkContext::~VkContext()
{
}

void VkContext::Initialize()
{
	this->SetupDebugCallback();
	this->PickPhysicalDevice();
	this->CreateLogicalDevice();
	this->CreateSwapchain();
	this->CreateImageViews();
	this->CreateRenderPass();
	this->CreateDescriptorSetLayout();
	//this->CreateGraphicsPipeline();
	this->CreateCommandPool();
	this->CreateColorResources();
	this->CreateDepthResources();
	this->CreateFrameBuffers();
	this->CreateUniformBuffer();
	this->CreateDescriptorPool();
	//this->CreateDescriptorSets();
	this->CreateCommandBuffers();
	this->CreateSyncObjects();

	// Setup Platform/Renderer bindings
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = m_instance;
	init_info.PhysicalDevice = m_physical_device;
	init_info.Device = m_device;
	init_info.QueueFamily = FindQueueFamilies(m_physical_device).graphics_familiy;
	init_info.Queue = m_graphics_queue;
	init_info.PipelineCache = nullptr;
	init_info.DescriptorPool = m_imgui_descriptor_pool;
	init_info.Allocator = nullptr;
	init_info.MinImageCount = MAX_FRAMES_IN_FLIGHT;
	init_info.ImageCount = MAX_FRAMES_IN_FLIGHT; //  m_vulkan_window->ImageCount;
	init_info.CheckVkResultFn = CheckVkResult;
	ImGui_ImplVulkan_Init(&init_info, m_render_pass);

	// Load Fonts
	VkCommandBuffer command_buffer = BeginSingleTimeCommands();
	ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
	EndSingleTimeCommands(command_buffer);

	ImGui_ImplVulkan_DestroyFontUploadObjects();

	m_renderer = std::make_unique<VkRenderer>();
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

	//vkDestroySampler(m_device, m_texture_sampler, nullptr);

	//vkDestroyImageView(m_device, m_texture_image_view, nullptr);

	//vkDestroyImage(m_device, m_texture_image, nullptr);
	//vkFreeMemory(m_device, m_texture_image_memory, nullptr);

	vkDestroyDescriptorPool(m_device, m_descriptor_pool, nullptr);

	vkDestroyDescriptorSetLayout(m_device, m_descriptor_set_layout, nullptr);

	//for (size_t i = 0; i < m_swapchain_images.size(); ++i)
	//{
	//	vkDestroyBuffer(m_device, m_uniform_buffers[i], nullptr);
	//	vkFreeMemory(m_device, m_uniform_buffers_memory[i], nullptr);
	//}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		vkDestroySemaphore(m_device, m_render_finished_semaphores[i], nullptr);
		vkDestroySemaphore(m_device, m_image_available_semaphores[i], nullptr);
		vkDestroyFence(m_device, m_in_flight_fences[i], nullptr);
	}

	vkDestroyCommandPool(m_device, m_command_pool, nullptr);
	vkDestroyDevice(m_device, nullptr);

	if (m_enable_validation_layers)
		vk_utils::DestroyDebugUtilsMessengerEXT(m_instance, m_debug_callback, nullptr);

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

void VkContext::BindResourcesToPipeline()
{
//	this->BindVertexArrayInternal(m_bound_vertex_array);
	vkCmdBindDescriptorSets(m_command_buffers[m_current_frame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline_layout, 0, 1, &m_descriptor_sets[m_current_frame], 0, nullptr);
}

void VkContext::Begin()
{
	this->CreateGraphicsPipeline();
	this->CreateDescriptorSets();


	vkWaitForFences(m_device, 1, &m_in_flight_fences[m_current_frame], VK_TRUE, std::numeric_limits<uint64_t>::max());
	uint32_t image_index;
	VkResult result = vkAcquireNextImageKHR(m_device, m_swapchain, std::numeric_limits<uint64_t>::max(), m_image_available_semaphores[m_current_frame], VK_NULL_HANDLE, &image_index);

	VkCommandBufferBeginInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	VKCALL(vkBeginCommandBuffer(m_command_buffers[image_index], &info));

	VkRenderPassBeginInfo render_pass_info = {};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	render_pass_info.renderPass = m_render_pass;
	render_pass_info.framebuffer = m_swapchain_frame_buffers[image_index];
	render_pass_info.renderArea.offset = { 0, 0 };
	render_pass_info.renderArea.extent = m_swapchain_extent;
	render_pass_info.clearValueCount = 1;

	std::array<VkClearValue, 2> clear_values = {};
	clear_values[0].color = { 1.0f, 0.0f, 0.0f, 1.0f };
	clear_values[1].depthStencil = { 1.0f, 0 };

	render_pass_info.clearValueCount = static_cast<uint32_t>(clear_values.size());
	render_pass_info.pClearValues = clear_values.data();

	vkCmdBeginRenderPass(m_command_buffers[image_index], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(m_command_buffers[image_index], VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphics_pipeline);
}

void VkContext::Present()
{
}

void VkContext::End()
{
	VkResult result = VK_SUCCESS;

	vkCmdEndRenderPass(m_command_buffers[m_current_frame]);
	VKCALL(vkEndCommandBuffer(m_command_buffers[m_current_frame]));

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapchain();
		return;
	}

	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		ASSERT(false, "failed to acquire swap chain image!");

	UpdateUniformBuffer(m_current_frame);
	VkSubmitInfo submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore wait_semaphores[] = { m_image_available_semaphores[m_current_frame] };
	VkPipelineStageFlags wait_stages[]{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submit_info.waitSemaphoreCount = 1;
	submit_info.pWaitSemaphores = wait_semaphores;
	submit_info.pWaitDstStageMask = wait_stages;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &m_command_buffers[m_current_frame];

	VkSemaphore signal_semaphores[] = { m_render_finished_semaphores[m_current_frame] };
	submit_info.signalSemaphoreCount = 1;
	submit_info.pSignalSemaphores = signal_semaphores;

	vkResetFences(m_device, 1, &m_in_flight_fences[m_current_frame]);

	VKCALL(vkQueueSubmit(m_graphics_queue, 1, &submit_info, m_in_flight_fences[m_current_frame]));

	VkPresentInfoKHR present_info = {};
	present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present_info.waitSemaphoreCount = 1;
	present_info.pWaitSemaphores = signal_semaphores;

	VkSwapchainKHR swapchains[] = { m_swapchain };

	present_info.swapchainCount = 1;
	present_info.pSwapchains = swapchains;
	present_info.pImageIndices = (uint32_t*)&m_current_frame;
	present_info.pResults = nullptr;

	result = vkQueuePresentKHR(m_present_queue, &present_info);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_is_frame_buffer_resized)
	{
		m_is_frame_buffer_resized = false;
		RecreateSwapchain();
	}
	else
		VKCALL(result);

	vkQueueWaitIdle(m_present_queue);
	m_current_frame = (m_current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}

Context::API VkContext::GetApiType() const
{
	return API::VULKAN;
}

VkDevice VkContext::GetDevice() const
{
	return m_device;
}

VkCommandBuffer VkContext::GetCurrentCommandBuffer() const
{
	return m_command_buffers[m_current_frame];
}

VkInstance VkContext::GetInstance() const
{
	return m_instance;
}

VkPhysicalDevice VkContext::GetSelectedGpu() const
{
	return m_physical_device;
}

void VkContext::CreateInstance(Window* window)
{
	if (m_enable_validation_layers && !this->CheckValidationLayerSupport())
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

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	//Message is important enough to show
	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		std::cerr << "fatal error" << std::endl;

	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

void VkContext::SetupDebugCallback()
{
	if (!m_enable_validation_layers)
		return;

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

	if (vk_utils::CreateDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debug_callback) != VK_SUCCESS)
		throw std::runtime_error("failed to set up debug callback!");
}

void VkContext::PickPhysicalDevice()
{
	uint32_t gpu_count = 0;
	vkEnumeratePhysicalDevices(m_instance, &gpu_count, nullptr);

	ASSERT(gpu_count, "failed to find gpus with Vulkan support!");

	std::vector<VkPhysicalDevice> gpus(gpu_count);
	vkEnumeratePhysicalDevices(m_instance, &gpu_count, gpus.data());

	for (const VkPhysicalDevice& gpu : gpus)
	{
		VkPhysicalDeviceProperties gpu_props;
		vkGetPhysicalDeviceProperties(gpu, &gpu_props);

		spdlog::info("checking {}", gpu_props.deviceName);

		if (IsGpuSuitable(gpu))
		{
			m_physical_device = gpu;
			m_msaa_samples = GetMaxUsableSampleCount();
		}
	}

	ASSERT(m_physical_device, "failed to find sitable gpu!");

	std::multimap<int, VkPhysicalDevice> candidates;

	for (const VkPhysicalDevice& gpu : gpus)
	{
		int score = RateDeviceSuitability(gpu);
		candidates.insert(std::make_pair(score, gpu));
	}

	if (candidates.rbegin()->first > 0)
		m_physical_device = candidates.rbegin()->second;
	else
		ASSERT(false, "failed to find suitable gpu!");

	VkPhysicalDeviceProperties gpu_props;
	vkGetPhysicalDeviceProperties(m_physical_device, &gpu_props);
	spdlog::info("using: {}", gpu_props.deviceName);
}

void VkContext::CreateSurface(Window* window)
{
	VKCALL(vk_utils::CreateWindowSurface(m_instance, window, nullptr, &m_surface));
}

void VkContext::CreateLogicalDevice()
{
	QueueFamilyIndices indices = FindQueueFamilies(m_physical_device);
	std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
	std::set<int> unique_queue_families = { indices.graphics_familiy, indices.present_family };

	float queue_priority = 1.0f;

	for (int queue_family : unique_queue_families)
	{
		VkDeviceQueueCreateInfo queue_create_info = {};
		queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_create_info.queueFamilyIndex = indices.graphics_familiy;
		queue_create_info.queueCount = 1;
		queue_create_info.pQueuePriorities = &queue_priority;

		queue_create_infos.emplace_back(queue_create_info);
	}

	VkDeviceCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	create_info.pQueueCreateInfos = queue_create_infos.data();
	create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());

	VkPhysicalDeviceFeatures device_features;
	vkGetPhysicalDeviceFeatures(m_physical_device, &device_features);
	device_features.samplerAnisotropy = VK_TRUE;
	device_features.sampleRateShading = VK_TRUE;

	create_info.pEnabledFeatures = &device_features;

	create_info.enabledExtensionCount = static_cast<uint32_t>(m_device_extentions.size());
	create_info.ppEnabledExtensionNames = m_device_extentions.data();

	if (m_enable_validation_layers)
	{
		create_info.enabledLayerCount = static_cast<uint32_t>(m_validation_layers.size());
		create_info.ppEnabledLayerNames = m_validation_layers.data();
	}
	else
		create_info.enabledLayerCount = 0;

	VKCALL(vkCreateDevice(m_physical_device, &create_info, nullptr, &m_device));

	vkGetDeviceQueue(m_device, indices.graphics_familiy, 0, &m_graphics_queue);
	vkGetDeviceQueue(m_device, indices.graphics_familiy, 0, &m_present_queue);
}

void VkContext::CreateSwapchain()
{
	SwapChainSupportDetails swapchain_support = QuerySwapchainSupport(m_physical_device);

	VkSurfaceFormatKHR surface_format = ChooseSwapchainSurfaceFormat(swapchain_support.formats);
	VkPresentModeKHR present_mode = ChooseSwapchainPresentMode(swapchain_support.present_modes);
	VkExtent2D extent = ChooseSwapExtent(swapchain_support.capabilities);

	uint32_t image_count = swapchain_support.capabilities.minImageCount + 1;
	if (swapchain_support.capabilities.minImageCount > 0 && image_count > swapchain_support.capabilities.maxImageCount)
		image_count = swapchain_support.capabilities.maxImageCount;

	VkSwapchainCreateInfoKHR create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	create_info.surface = m_surface;
	create_info.minImageCount = image_count;
	create_info.minImageCount = image_count;
	create_info.imageFormat = surface_format.format;
	create_info.imageColorSpace = surface_format.colorSpace;
	create_info.imageExtent = extent;
	create_info.imageArrayLayers = 1;
	create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = FindQueueFamilies(m_physical_device);
	uint32_t queue_family_indices[] = { (uint32_t)indices.graphics_familiy, (uint32_t)indices.present_family };

	if (indices.graphics_familiy != indices.present_family)
	{
		create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		create_info.queueFamilyIndexCount = 2;
		create_info.pQueueFamilyIndices = queue_family_indices;
	}
	else
	{
		create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		create_info.queueFamilyIndexCount = 0;
		create_info.pQueueFamilyIndices = nullptr;
	}

	create_info.preTransform = swapchain_support.capabilities.currentTransform;
	create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	create_info.presentMode = present_mode;
	create_info.clipped = VK_TRUE;

	create_info.oldSwapchain = VK_NULL_HANDLE;

	VKCALL(vkCreateSwapchainKHR(m_device, &create_info, nullptr, &m_swapchain));

	vkGetSwapchainImagesKHR(m_device, m_swapchain, &image_count, nullptr);
	m_swapchain_images.resize(image_count);
	vkGetSwapchainImagesKHR(m_device, m_swapchain, &image_count, m_swapchain_images.data());

	m_swapchain_image_format = surface_format.format;
	m_swapchain_extent = extent;
}

void VkContext::CreateImageViews()
{
	m_swapchain_image_views.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < m_swapchain_images.size(); ++i)
		m_swapchain_image_views[i] = CreateImageView(m_swapchain_images[i], m_swapchain_image_format, VK_IMAGE_ASPECT_COLOR_BIT, 1);
}

void VkContext::CreateRenderPass()
{
	VkAttachmentDescription color_attachment = {};
	color_attachment.format = m_swapchain_image_format;
	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT; // needs to be this for imgui! aka no multi sampling
	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	//VkAttachmentDescription color_attachment_resolve = {};
	//color_attachment_resolve.format = m_swapchain_image_format;
	//color_attachment_resolve.samples = VK_SAMPLE_COUNT_1_BIT;
	//color_attachment_resolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	//color_attachment_resolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	//color_attachment_resolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	//color_attachment_resolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	//color_attachment_resolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	//color_attachment_resolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	//VkAttachmentDescription depth_attachment = {};
	//depth_attachment.format = FindDepthFormat();
	//depth_attachment.samples = m_msaa_samples;
	//depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	//depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	//depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	//depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	//depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	//depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

	VkAttachmentReference color_attachment_ref = {};
	color_attachment_ref.attachment = 0;
	color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	//VkAttachmentReference color_attachment_resolve_ref = {};
	//color_attachment_resolve_ref.attachment = 2;
	//color_attachment_resolve_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	//VkAttachmentReference depth_attachment_ref = {};
	//depth_attachment_ref.attachment = 1;
	//depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_attachment_ref;
	//subpass.pDepthStencilAttachment = &depth_attachment_ref;
	//subpass.pResolveAttachments = &color_attachment_resolve_ref;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 1> attachments = { color_attachment };// , depth_attachment, color_attachment_resolve

	VkRenderPassCreateInfo render_pass_info = {};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_info.attachmentCount = static_cast<uint32_t>(attachments.size());
	render_pass_info.pAttachments = attachments.data();
	render_pass_info.subpassCount = 1;
	render_pass_info.pSubpasses = &subpass;
	render_pass_info.dependencyCount = 1;
	render_pass_info.pDependencies = &dependency;

	VKCALL(vkCreateRenderPass(m_device, &render_pass_info, nullptr, &m_render_pass));
	VKCALL(vkCreateRenderPass(m_device, &render_pass_info, nullptr, &m_imgui_render_pass));

}

void VkContext::CreateDescriptorSetLayout()
{
	VkDescriptorSetLayoutBinding ubo_layout_binding = {};
	ubo_layout_binding.binding = 0;
	ubo_layout_binding.descriptorCount = 1;
	ubo_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	ubo_layout_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	ubo_layout_binding.pImmutableSamplers = nullptr;

	//VkDescriptorSetLayoutBinding sampler_layout_binding = {};
	//sampler_layout_binding.binding = 1;
	//sampler_layout_binding.descriptorCount = 1;
	//sampler_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	//sampler_layout_binding.pImmutableSamplers = nullptr;

	//sampler_layout_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 1> bindings = { ubo_layout_binding };

	VkDescriptorSetLayoutCreateInfo layout_info = {};
	layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_info.bindingCount = static_cast<uint32_t>(bindings.size());
	layout_info.pBindings = bindings.data();

	VKCALL(vkCreateDescriptorSetLayout(m_device, &layout_info, nullptr, &m_descriptor_set_layout));
}

void VkContext::CreateGraphicsPipeline()
{
	if (m_graphics_pipeline)
		return;

	VkPipelineInputAssemblyStateCreateInfo input_assembly = {};
	input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	input_assembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)m_swapchain_extent.width;
	viewport.height = (float)m_swapchain_extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0,0 };
	scissor.extent = m_swapchain_extent;

	VkPipelineViewportStateCreateInfo viewport_state = {};
	viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_state.viewportCount = 1;
	viewport_state.pViewports = &viewport;
	viewport_state.scissorCount = 1;
	viewport_state.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_NONE;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f;
	rasterizer.depthBiasClamp = 0.0f;
	rasterizer.depthBiasSlopeFactor = 0.0f;

	VkPipelineMultisampleStateCreateInfo multi_sampling = {};
	multi_sampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multi_sampling.sampleShadingEnable = VK_TRUE;
	multi_sampling.rasterizationSamples = m_msaa_samples;
	multi_sampling.minSampleShading = 0.2f;
	multi_sampling.pSampleMask = nullptr;
	multi_sampling.alphaToCoverageEnable = VK_FALSE;
	multi_sampling.alphaToOneEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState color_blend_attachment = {};
	color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	color_blend_attachment.blendEnable = VK_FALSE;
	color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
	color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo color_blending = {};
	color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blending.logicOpEnable = VK_FALSE;
	color_blending.logicOp = VK_LOGIC_OP_COPY;
	color_blending.attachmentCount = 1;
	color_blending.pAttachments = &color_blend_attachment;
	color_blending.blendConstants[0] = 0.0f;
	color_blending.blendConstants[1] = 0.0f;
	color_blending.blendConstants[2] = 0.0f;
	color_blending.blendConstants[3] = 0.0f;

	VkDynamicState dynamic_states[] =
	{
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH
	};

	VkPipelineDynamicStateCreateInfo dynamic_state = {};
	dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamic_state.dynamicStateCount = 2;
	dynamic_state.pDynamicStates = dynamic_states;

	VkPipelineLayoutCreateInfo pipeline_layout_info = {};
	pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipeline_layout_info.setLayoutCount = 1;
	pipeline_layout_info.pSetLayouts = &m_descriptor_set_layout;
	pipeline_layout_info.pushConstantRangeCount = 0;
	pipeline_layout_info.pPushConstantRanges = nullptr;

	VKCALL(vkCreatePipelineLayout(m_device, &pipeline_layout_info, nullptr, &m_pipeline_layout));

	VkPipelineDepthStencilStateCreateInfo depth_stencil = {};
	depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depth_stencil.depthTestEnable = VK_TRUE;
	depth_stencil.depthWriteEnable = VK_TRUE;
	depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depth_stencil.depthBoundsTestEnable = VK_FALSE;
	depth_stencil.minDepthBounds = 0.0f;
	depth_stencil.maxDepthBounds = 1.0f;
	depth_stencil.stencilTestEnable = VK_FALSE;
	depth_stencil.front = {};
	depth_stencil.back = {};

	VkGraphicsPipelineCreateInfo pipeline_info = {};
	pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_info.stageCount = 2;
	pipeline_info.pStages = static_cast<const VkShaderProgram*>(m_bound_shader_program)->GetPipelineShaderStageCreateInfos().data();

	const VkVertexLayout* vk_layout = static_cast<const VkVertexLayout*>(m_bound_vertex_array->GetVertexLayout());
	pipeline_info.pVertexInputState = &vk_layout->GetCreateInfo();
	pipeline_info.pInputAssemblyState = &input_assembly;
	pipeline_info.pViewportState = &viewport_state;
	pipeline_info.pRasterizationState = &rasterizer;
	pipeline_info.pMultisampleState = &multi_sampling;
	pipeline_info.pDepthStencilState = &depth_stencil;
	pipeline_info.pColorBlendState = &color_blending;
	pipeline_info.pDynamicState = nullptr;

	pipeline_info.layout = m_pipeline_layout;
	pipeline_info.renderPass = m_render_pass;
	pipeline_info.subpass = 0;
	pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
	pipeline_info.basePipelineIndex = -1;

	VKCALL(vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_graphics_pipeline));
}

void VkContext::CreateCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(m_physical_device);

	VkCommandPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.queueFamilyIndex = queueFamilyIndices.graphics_familiy;
	pool_info.flags = 0;

	VKCALL(vkCreateCommandPool(m_device, &pool_info, nullptr, &m_command_pool));
}

void VkContext::CreateColorResources()
{
	VkFormat color_format = m_swapchain_image_format;
	CreateImage(m_swapchain_extent.width, m_swapchain_extent.height, 1,
		m_msaa_samples, color_format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT |
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		m_color_image, m_color_image_memory);

	m_color_image_view = CreateImageView(m_color_image, color_format, VK_IMAGE_ASPECT_COLOR_BIT, 1);

	TransitionImageLayout(m_color_image, color_format, VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 1);
}

void VkContext::CreateDepthResources()
{
	VkFormat depth_format = FindDepthFormat();
	CreateImage(m_swapchain_extent.width, m_swapchain_extent.height, 1, m_msaa_samples, depth_format,
		VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_depth_image, m_depth_image_memory);
	m_depth_image_view = CreateImageView(m_depth_image, depth_format, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

	TransitionImageLayout(m_depth_image, depth_format, VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);
}

void VkContext::CreateFrameBuffers()
{
	m_swapchain_frame_buffers.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		//std::array<VkImageView, 3> attachments =
		//{
		//	m_color_image_view,
		//	m_depth_image_view,
		//	m_swapchain_image_views[i]
		//};

		std::array<VkImageView, 1> attachments =
		{
			m_swapchain_image_views[i]
		};

		VkFramebufferCreateInfo framebuffer_info = {};
		framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebuffer_info.renderPass = m_render_pass;
		framebuffer_info.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebuffer_info.pAttachments = attachments.data();

		framebuffer_info.width = m_swapchain_extent.width;
		framebuffer_info.height = m_swapchain_extent.height;
		framebuffer_info.layers = 1;

		VKCALL(vkCreateFramebuffer(m_device, &framebuffer_info, nullptr, &m_swapchain_frame_buffers[i]));
	}
}

//void VkContext::CreateTextureImage()
//{
//	int tex_width, tex_height, tex_channels;
//
//	stbi_uc* pixels = stbi_load(TEXTURE_PATH.c_str(), &tex_width, &tex_height, &tex_channels, STBI_rgb_alpha);
//
//	VkDeviceSize image_size = tex_width * tex_height * sizeof(char) * 4; //4 bytes per pixel
//	m_mip_levels = static_cast<uint32_t>(std::floor(std::log2(std::max(tex_width, tex_height)))) + 1;
//
//	if (!pixels)
//		ThrowException("failed to load texture image!");
//
//	VkBuffer staging_buffer;
//	VkDeviceMemory staging_buffer_memory;
//
//	CreateBuffer(image_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer, staging_buffer_memory);
//
//	void* data;
//	vkMapMemory(m_device, staging_buffer_memory, 0, image_size, 0, &data);
//	memcpy(data, pixels, static_cast<size_t>(image_size));
//	vkUnmapMemory(m_device, staging_buffer_memory);
//
//	stbi_image_free(pixels);
//
//	CreateImage(tex_width, tex_height, m_mip_levels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_UNORM,
//		VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_texture_image, m_texture_image_memory);
//
//	TransitionImageLayout(m_texture_image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_mip_levels);
//
//	CopyBufferToImage(staging_buffer, m_texture_image, static_cast<uint32_t>(tex_width), static_cast<uint32_t>(tex_height));
//
//	GenerateMipMaps(m_texture_image, VK_FORMAT_R8G8B8A8_UNORM, tex_width, tex_height, m_mip_levels);
//	vkDestroyBuffer(m_device, staging_buffer, nullptr);
//	vkFreeMemory(m_device, staging_buffer_memory, nullptr);
//}
//
//void VkContext::CreateTextureImageView()
//{
//	m_texture_image_view = CreateImageView(m_texture_image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, m_mip_levels);
//}
//
//void VkContext::CreateTextureSampler()
//{
//	VkSamplerCreateInfo sampler_info = {};
//	sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
//	sampler_info.magFilter = VK_FILTER_LINEAR;
//	sampler_info.minFilter = VK_FILTER_LINEAR;
//	sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
//	sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
//	sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
//	sampler_info.anisotropyEnable = VK_TRUE;
//	sampler_info.maxAnisotropy = 16;
//	sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
//	sampler_info.unnormalizedCoordinates = VK_FALSE;
//	sampler_info.compareEnable = VK_FALSE;
//	sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;
//	sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
//	sampler_info.mipLodBias = 0.0f;
//	sampler_info.minLod = 0.0f;
//	sampler_info.maxLod = static_cast<float>(m_mip_levels);
//
//	VKCALL(vkCreateSampler(m_device, &sampler_info, nullptr, &m_texture_sampler));
//}

//void VkContext::LoadModel()
//{
//	tinyobj::attrib_t attrib;
//	std::vector<tinyobj::shape_t> shapes;
//	std::vector<tinyobj::material_t> materials;
//	std::string warning;
//	std::string error;
//
//	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, MODEL_PATH.c_str()))
//		ASSERT(false, error);
//
//	std::unordered_map<Vertex, uint32_t> unique_vertices = {};
//
//	for (const tinyobj::shape_t& shape : shapes)
//	{
//		for (const tinyobj::index_t& index : shape.mesh.indices)
//		{
//			Vertex vertex = {};
//			vertex.Position =
//			{
//				attrib.vertices[3 * index.vertex_index + 0],
//				attrib.vertices[3 * index.vertex_index + 1],
//				attrib.vertices[3 * index.vertex_index + 2]
//			};
//
//			vertex.TexCoord =
//			{
//				attrib.texcoords[2 * index.texcoord_index + 0],
//				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
//			};
//
//			vertex.Color = { 1.0f, 1.0f, 1.0f };
//
//			if (unique_vertices.count(vertex) == 0)
//			{
//				unique_vertices[vertex] = static_cast<uint32_t>(m_vertices.size());
//				m_vertices.push_back(vertex);
//			}
//
//			m_indices.push_back(unique_vertices[vertex]);
//		}
//	}
//}

void VkContext::CreateUniformBuffer()
{
	//VkDeviceSize buffer_size = sizeof(UniformBufferObject);

	//m_uniform_buffers.resize(m_swapchain_images.size());
	//m_uniform_buffers_memory.resize(m_swapchain_images.size());

	//for (size_t i = 0; i < m_swapchain_images.size(); ++i)
	//{
	//	CreateBuffer(buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniform_buffers[i], m_uniform_buffers_memory[i]);
	//}
}

void VkContext::CreateDescriptorPool()
{
	std::array<VkDescriptorPoolSize, 2> pool_sizes = {};
	pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	pool_sizes[0].descriptorCount = static_cast<uint32_t>(m_swapchain_images.size());

	pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	pool_sizes[1].descriptorCount = static_cast<uint32_t>(m_swapchain_images.size());

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.poolSizeCount = static_cast<uint32_t>(pool_sizes.size());
	pool_info.pPoolSizes = pool_sizes.data();
	pool_info.maxSets = static_cast<uint32_t>(m_swapchain_images.size());

	VKCALL(vkCreateDescriptorPool(m_device, &pool_info, nullptr, &m_descriptor_pool));

	VkDescriptorPoolSize pool_sizes_arr[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};
	pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes_arr);
	pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes_arr);
	pool_info.pPoolSizes = pool_sizes_arr;
	VKCALL(vkCreateDescriptorPool(m_device, &pool_info, nullptr, &m_imgui_descriptor_pool));
}

void VkContext::CreateDescriptorSets()
{
	if (!m_descriptor_sets.empty())
		return;

	std::vector<VkDescriptorSetLayout> layouts(m_swapchain_images.size(), m_descriptor_set_layout);

	VkDescriptorSetAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	alloc_info.descriptorPool = m_descriptor_pool;
	alloc_info.descriptorSetCount = static_cast<uint32_t>(m_swapchain_images.size());
	alloc_info.pSetLayouts = layouts.data();

	m_descriptor_sets.resize(m_swapchain_images.size());

	VKCALL(vkAllocateDescriptorSets(m_device, &alloc_info, &m_descriptor_sets[0]));

	for (size_t i = 0; i < m_swapchain_images.size(); ++i)
	{
		VkDescriptorBufferInfo buffer_info = {};
		buffer_info.buffer = static_cast<const VkShaderProgram*>(m_bound_shader_program)->GetUniformBuffer();
		buffer_info.offset = 0;
		buffer_info.range = sizeof(UniformBufferObject);

		//VkDescriptorImageInfo image_info = {};
		//image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		//image_info.imageView = m_texture_image_view;
		//image_info.sampler = m_texture_sampler;

		std::array<VkWriteDescriptorSet, 1> descriptor_writes = {};
		descriptor_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptor_writes[0].dstSet = m_descriptor_sets[i];
		descriptor_writes[0].dstBinding = 0;
		descriptor_writes[0].dstArrayElement = 0;
		descriptor_writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptor_writes[0].descriptorCount = 1;
		descriptor_writes[0].pBufferInfo = &buffer_info;
		descriptor_writes[0].descriptorCount = 1;
		descriptor_writes[0].pBufferInfo = &buffer_info;
		descriptor_writes[0].pImageInfo = nullptr;
		descriptor_writes[0].pTexelBufferView = nullptr;

		//descriptor_writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		//descriptor_writes[1].dstSet = m_descriptor_sets[i];
		//descriptor_writes[1].dstBinding = 1;
		//descriptor_writes[1].dstArrayElement = 0;
		//descriptor_writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		//descriptor_writes[1].descriptorCount = 1;

		//descriptor_writes[1].pImageInfo = &image_info;

		vkUpdateDescriptorSets(m_device, static_cast<uint32_t>(descriptor_writes.size()), descriptor_writes.data(), 0, nullptr);
	}
}

void VkContext::CreateCommandBuffers()
{
	m_command_buffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.commandPool = m_command_pool;
	alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_info.commandBufferCount = (uint32_t)m_command_buffers.size();

	VKCALL(vkAllocateCommandBuffers(m_device, &alloc_info, m_command_buffers.data()));

	for (size_t i = 0; i < m_command_buffers.size(); ++i)
	{
		VkCommandBufferBeginInfo begin_info = {};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		begin_info.pInheritanceInfo = nullptr;

		VKCALL(vkBeginCommandBuffer(m_command_buffers[i], &begin_info));

		VkRenderPassBeginInfo render_pass_info = {};
		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_info.renderPass = m_render_pass;
		render_pass_info.framebuffer = m_swapchain_frame_buffers[i];
		render_pass_info.renderArea.offset = { 0,0 };
		render_pass_info.renderArea.extent = m_swapchain_extent;

		std::array<VkClearValue, 2> clear_values = {};
		clear_values[0].color = { 1.0f, 0.0f, 0.0f, 1.0f };
		clear_values[1].depthStencil = { 1.0f, 0 };

		render_pass_info.clearValueCount = static_cast<uint32_t>(clear_values.size());
		render_pass_info.pClearValues = clear_values.data();
	}
}

void VkContext::CreateSyncObjects()
{
	m_image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
	m_render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
	m_in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphore_info = {};
	semaphore_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

	VkFenceCreateInfo fence_info = {};
	fence_info.flags = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		VKCALL(vkCreateSemaphore(m_device, &semaphore_info, nullptr, &m_image_available_semaphores[i]));
		VKCALL(vkCreateSemaphore(m_device, &semaphore_info, nullptr, &m_render_finished_semaphores[i]));
		VKCALL(vkCreateFence(m_device, &fence_info, nullptr, &m_in_flight_fences[i]));
	}
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

void VkContext::CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
	VkImageCreateInfo image_info = {};
	image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image_info.imageType = VK_IMAGE_TYPE_2D;
	image_info.extent.width = width;
	image_info.extent.height = height;
	image_info.extent.depth = 1;
	image_info.arrayLayers = 1;
	image_info.format = format;
	image_info.tiling = tiling;
	image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	image_info.usage = usage;
	image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	image_info.samples = numSamples;
	image_info.flags = 0;
	image_info.mipLevels = mipLevels;

	VKCALL(vkCreateImage(m_device, &image_info, nullptr, &image));

	VkMemoryRequirements mem_requirements;
	vkGetImageMemoryRequirements(m_device, image, &mem_requirements);

	VkMemoryAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	alloc_info.allocationSize = mem_requirements.size;
	alloc_info.memoryTypeIndex = FindMemoryType(mem_requirements.memoryTypeBits, properties);

	VKCALL(vkAllocateMemory(m_device, &alloc_info, nullptr, &imageMemory));

	vkBindImageMemory(m_device, image, imageMemory, 0);
}

bool VkContext::IsGpuSuitable(const VkPhysicalDevice& gpu)
{
	QueueFamilyIndices indices = FindQueueFamilies(gpu);

	bool extention_supported = CheckDeviceExtensionSupport(gpu);

	bool swapchain_adequate = false;
	if (extention_supported)
	{
		SwapChainSupportDetails swapchain_support = QuerySwapchainSupport(gpu);
		swapchain_adequate = !swapchain_support.formats.empty() && !swapchain_support.present_modes.empty();
	}

	VkPhysicalDeviceFeatures supported_features;
	vkGetPhysicalDeviceFeatures(gpu, &supported_features);

	return indices.IsComplete() && extention_supported && swapchain_adequate && supported_features.samplerAnisotropy;
}

QueueFamilyIndices VkContext::FindQueueFamilies(VkPhysicalDevice gpu)
{
	QueueFamilyIndices indices;

	uint32_t queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queue_family_count, nullptr);

	std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queue_family_count, queue_families.data());

	int i = 0;
	for (const VkQueueFamilyProperties& queue_family : queue_families)
	{
		if (queue_family.queueCount > 0 && queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indices.graphics_familiy = i;

		if (indices.IsComplete())
			break;

		VkBool32 present_support = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(gpu, i, m_surface, &present_support);
		if (queue_family.queueCount >= 0 && present_support)
			indices.present_family = i;

		++i;
	}

	return indices;
}

int VkContext::RateDeviceSuitability(VkPhysicalDevice gpu)
{
	VkPhysicalDeviceProperties gpu_properties;
	vkGetPhysicalDeviceProperties(gpu, &gpu_properties);

	VkPhysicalDeviceFeatures gpu_features;
	vkGetPhysicalDeviceFeatures(gpu, &gpu_features);

	return gpu_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && gpu_features.geometryShader;

	int score = 0;

	if (gpu_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		score += 1000;

	score += gpu_properties.limits.maxImageDimension2D;

	if (!gpu_features.geometryShader)
		return 0;

	return score;
}

SwapChainSupportDetails VkContext::QuerySwapchainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities);

	uint32_t format_count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &format_count, nullptr);

	if (format_count != 0)
	{
		details.formats.resize(format_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &format_count, details.formats.data());
	}

	uint32_t present_mode_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &present_mode_count, nullptr);

	if (present_mode_count != 0)
	{
		details.present_modes.resize(present_mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &present_mode_count, details.present_modes.data());
	}

	return details;
}

VkSurfaceFormatKHR VkContext::ChooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
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
VkPresentModeKHR VkContext::ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
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

VkExtent2D VkContext::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR & capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		return capabilities.currentExtent;

	else
		ASSERT(false, "current extent is invalid!");

	return {};
}

VkImageView VkContext::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
{
	VkImageViewCreateInfo view_info = {};
	view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	view_info.image = image;
	view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	view_info.format = format;

	view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	view_info.subresourceRange.aspectMask = aspectFlags;
	view_info.subresourceRange.baseMipLevel = 0;
	view_info.subresourceRange.levelCount = mipLevels;
	view_info.subresourceRange.baseArrayLayer = 0;
	view_info.subresourceRange.levelCount = 1;

	VkImageView image_view;
	VKCALL(vkCreateImageView(m_device, &view_info, nullptr, &image_view));

	return image_view;
}

VkSampleCountFlagBits VkContext::GetMaxUsableSampleCount()
{
	VkPhysicalDeviceProperties physical_device_properties;
	vkGetPhysicalDeviceProperties(m_physical_device, &physical_device_properties);

	VkSampleCountFlags counts = std::min(physical_device_properties.limits.framebufferColorSampleCounts, physical_device_properties.limits.framebufferDepthSampleCounts);

	if (counts & VK_SAMPLE_COUNT_64_BIT)
		return VK_SAMPLE_COUNT_64_BIT;
	else if (counts & VK_SAMPLE_COUNT_32_BIT)
		return VK_SAMPLE_COUNT_32_BIT;
	else if (counts & VK_SAMPLE_COUNT_8_BIT)
		return VK_SAMPLE_COUNT_8_BIT;
	else if (counts & VK_SAMPLE_COUNT_4_BIT)
		return VK_SAMPLE_COUNT_4_BIT;
	else if (counts & VK_SAMPLE_COUNT_2_BIT)
		return VK_SAMPLE_COUNT_2_BIT;

	return VK_SAMPLE_COUNT_1_BIT;
}

VkFormat VkContext::FindDepthFormat()
{
	return FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

//void VkContext::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
//{
//	VkBufferCreateInfo buffer_info = {};
//	buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//	buffer_info.size = size;
//	buffer_info.usage = usage;
//	buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
//
//	VKCALL(vkCreateBuffer(m_device, &buffer_info, nullptr, &buffer));
//
//	VkMemoryRequirements mem_requirements;
//	vkGetBufferMemoryRequirements(m_device, buffer, &mem_requirements);
//
//	VkMemoryAllocateInfo alloc_info = {};
//	alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//	alloc_info.allocationSize = mem_requirements.size;
//	alloc_info.memoryTypeIndex = FindMemoryType(mem_requirements.memoryTypeBits, properties);
//
//	VKCALL(vkAllocateMemory(m_device, &alloc_info, nullptr, &bufferMemory));
//
//	vkBindBufferMemory(m_device, buffer, bufferMemory, 0);
//}

void VkContext::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels)
{
	VkCommandBuffer command_buffer = BeginSingleTimeCommands();

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = mipLevels;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = 0;
	if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		if (vk_utils::HasStencilComponent(format))
			barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
	}
	else
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

	VkPipelineStageFlags source_stage;
	VkPipelineStageFlags destination_stage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destination_stage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destination_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	}
	else
		ThrowException("unsupported layout transition!");

	vkCmdPipelineBarrier(command_buffer,
		source_stage, destination_stage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier);

	EndSingleTimeCommands(command_buffer);

}

void VkContext::GenerateMipMaps(VkImage image, VkFormat format, int32_t texWidth, int32_t texHeight, uint32_t mipLevels)
{
	VkFormatProperties format_props;
	vkGetPhysicalDeviceFormatProperties(m_physical_device, format, &format_props);

	if (!(format_props.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
		ThrowException("txture image format does not support linear blitting!");

	VkCommandBuffer command_buffer = BeginSingleTimeCommands();
	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstAccessMask = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;

	int32_t mip_width = texWidth;
	int32_t mip_height = texHeight;

	for (uint32_t i = 1; i < mipLevels; ++i)
	{
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		VkImageBlit blit = {};
		blit.srcOffsets[0] = { 0,0,0 };
		blit.srcOffsets[1] = { mip_width, mip_height };
		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = { 0,0,0 };
		blit.srcOffsets[1] = { mip_width > 1 ? static_cast<int32_t>(mip_width * 0.5f) : 1, mip_height > 1 ? static_cast<int32_t>(mip_height * 0.5f) : 1, 1 };
		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		vkCmdBlitImage(command_buffer,
			image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &blit, VK_FILTER_LINEAR);

		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		if (mip_width > 1) mip_width /= 2;
		if (mip_height > 1) mip_height /= 2;

	}

	barrier.subresourceRange.baseMipLevel = 0; // m_mip_levels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	EndSingleTimeCommands(command_buffer);
}

void VkContext::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
	VkCommandBuffer command_buffer = BeginSingleTimeCommands();
	VkBufferImageCopy region = {};

	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = { 0,0,0 };
	region.imageExtent = { width, height, 1 };

	vkCmdCopyBufferToImage(command_buffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	EndSingleTimeCommands(command_buffer);
}

uint32_t VkContext::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties mem_properties;
	vkGetPhysicalDeviceMemoryProperties(m_physical_device, &mem_properties);

	for (uint32_t i = 0; i < mem_properties.memoryTypeCount; ++i)
	{
		if (typeFilter & (1 << i) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	}

	ThrowException("failed to find suitable memory type!");
}

bool VkContext::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extension_count;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
	std::vector<VkExtensionProperties> available_extensions(extension_count);

	vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

	std::set<std::string> required_extensions(m_device_extentions.begin(), m_device_extentions.end());

	for (const VkExtensionProperties& extension : available_extensions)
		required_extensions.erase(extension.extensionName);

	return required_extensions.empty();
}

//void VkContext::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
//{
//	VkCommandBuffer command_buffer = BeginSingleTimeCommands();
//
//	VkBufferCopy copy_region = {};
//	copy_region.srcOffset = 0;
//	copy_region.dstOffset = 0;
//	copy_region.size = size;
//
//	vkCmdCopyBuffer(command_buffer, srcBuffer, dstBuffer, 1, &copy_region);
//
//	EndSingleTimeCommands(command_buffer);
//}

VkFormat VkContext::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	//The support of a format depends on the tiling mode and usage, so we must also include these as parameters.
	//The support of a format can be queried using the vkGetPhysicalDeviceFormatPropererties function
	for (VkFormat format : candidates)
	{
		//VkFormatProperties contains 3 fields:
		//linearTilingFeatures: Use cases that are supported with linear tiling
		//optimalTilingFeatures: USe cases that are supported with optimal tiling
		//bufferFeatures: Use cases that are supported for buffers.
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(m_physical_device, format, &props);

		//Only the first 2 fields are relevant here, and the one we check depends on the tiling parameter
		//of the function
		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
			return format;
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
			return format;

		//If none of the candidate formats support the desired usage, then we can either return a special
		//value of simply throw an exception
	}

	throw std::runtime_error("failed to find supported format!");
}

VkCommandBuffer VkContext::BeginSingleTimeCommands()
{
	VkCommandBufferAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_info.commandPool = m_command_pool;
	alloc_info.commandBufferCount = 1;

	VkCommandBuffer command_buffer;
	vkAllocateCommandBuffers(m_device, &alloc_info, &command_buffer);

	VkCommandBufferBeginInfo begin_info = {};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(command_buffer, &begin_info);
	return command_buffer;
}

void VkContext::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
{
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(m_graphics_queue, 1, &submit_info, VK_NULL_HANDLE);
	vkQueueWaitIdle(m_graphics_queue);

	vkFreeCommandBuffers(m_device, m_command_pool, 1, &commandBuffer);
}

void VkContext::BindIndexBufferInternal(const IndexBuffer* indexBuffer)
{
	vkCmdBindIndexBuffer(m_command_buffers[m_current_frame], static_cast<const VkIndexBuffer*>(m_bound_index_buffer)->GetGpuBuffer(), 0, VK_INDEX_TYPE_UINT32);
}

void VkContext::UnbindIndexBufferInternal(const IndexBuffer* indexBuffer)
{
}

void VkContext::BindVertexArrayInternal(const VertexArray* vertexArray)
{
	const VkVertexBuffer* vk_vb = static_cast<const VkVertexBuffer*>(vertexArray->GetVertexBuffer());

	VkBuffer vertex_buffers[] = { vk_vb->GetGpuBuffer() };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(m_command_buffers[m_current_frame], 0, 1, vertex_buffers, offsets);
}

void VkContext::UnbindVertexArrayInternal(const VertexArray* vertexArray)
{
}

void VkContext::BindShaderProgramInternal(const ShaderProgram* shaderProgram)
{
}

void VkContext::UnbindShaderProgramInternal(const ShaderProgram* shaderProgram)
{
}

void VkContext::RecreateSwapchain()
{
	vkDeviceWaitIdle(m_device);
	CleanupSwapchain();
	CreateSwapchain();
	CreateImageViews();
	CreateRenderPass();
	CreateGraphicsPipeline();
	CreateColorResources();
	CreateDepthResources();
	CreateFrameBuffers();
	CreateCommandBuffers();
}

void VkContext::UpdateUniformBuffer(uint32_t imageIndex)
{
	m_bound_shader_program->UploadVariables();
}


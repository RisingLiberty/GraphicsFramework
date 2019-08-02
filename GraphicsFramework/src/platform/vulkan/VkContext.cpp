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
#include "VkCommandQueue.h"
#include "VkCommandList.h"

#include "VkGpu.h"
#include "VkDefaultDevice.h"

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
	Cleanup();
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
	this->CreateColorResources();
	this->CreateDepthResources();
	this->CreateFrameBuffers();
	this->CreateUniformBuffer();
	this->CreateDescriptorPool();
	//this->CreateDescriptorSets();

	// Setup Platform/Renderer bindings
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = m_instance;
	init_info.PhysicalDevice = m_gpu->GetPhysicalDevice();
	init_info.Device = m_device->GetApiDevice();
	init_info.QueueFamily = m_gpu->GetGraphicsPresentQueueIndex();
	init_info.Queue = m_command_queue->GetApiQueue();
	init_info.PipelineCache = nullptr;
	init_info.DescriptorPool = m_imgui_descriptor_pool;
	init_info.Allocator = nullptr;
	init_info.MinImageCount = MAX_FRAMES_IN_FLIGHT;
	init_info.ImageCount = MAX_FRAMES_IN_FLIGHT; //  m_vulkan_window->ImageCount;
	init_info.CheckVkResultFn = CheckVkResult;
	ImGui_ImplVulkan_Init(&init_info, m_render_pass);

	// Load Fonts
	std::unique_ptr<VkCommandList> command_buffer = BeginSingleTimeCommands();
	ImGui_ImplVulkan_CreateFontsTexture(command_buffer->GetApiBuffer());
	EndSingleTimeCommands(command_buffer);

	ImGui_ImplVulkan_DestroyFontUploadObjects();

	m_renderer = std::make_unique<VkRenderer>();
}

void VkContext::Cleanup()
{
	Context::CleanUp();

	//Clean vulkan
	this->CleanupSwapchain();

	vkDestroyImageView(m_device->GetApiDevice(), m_color_image_view, nullptr);
	vkDestroyImage(m_device->GetApiDevice(), m_color_image, nullptr);
	vkFreeMemory(m_device->GetApiDevice(), m_color_image_memory, nullptr);

	vkDestroyImage(m_device->GetApiDevice(), m_depth_image, nullptr);
	vkDestroyImageView(m_device->GetApiDevice(), m_depth_image_view, nullptr);
	vkFreeMemory(m_device->GetApiDevice(), m_depth_image_memory, nullptr);

	//vkDestroySampler(m_device, m_texture_sampler, nullptr);

	//vkDestroyImageView(m_device, m_texture_image_view, nullptr);

	//vkDestroyImage(m_device, m_texture_image, nullptr);
	//vkFreeMemory(m_device, m_texture_image_memory, nullptr);

	vkDestroyDescriptorPool(m_device->GetApiDevice(), m_descriptor_pool, nullptr);

	vkDestroyDescriptorSetLayout(m_device->GetApiDevice(), m_descriptor_set_layout, nullptr);

	//for (size_t i = 0; i < m_swapchain_images.size(); ++i)
	//{
	//	vkDestroyBuffer(m_device, m_uniform_buffers[i], nullptr);
	//	vkFreeMemory(m_device, m_uniform_buffers_memory[i], nullptr);
	//}

	m_command_queue.reset();
	m_device.reset();

	vk_utils::DestroyDebugUtilsMessengerEXT(m_instance, m_debug_callback, nullptr);

	vkDestroySurfaceKHR(m_instance, m_surface, nullptr);

	vkDestroyInstance(m_instance, nullptr);
}

void VkContext::CleanupSwapchain()
{
	for (size_t i = 0; i < m_swapchain_frame_buffers.size(); ++i)
		vkDestroyFramebuffer(m_device->GetApiDevice(), m_swapchain_frame_buffers[i], nullptr);

	vkDestroyPipeline(m_device->GetApiDevice(), m_graphics_pipeline, nullptr);
	vkDestroyPipelineLayout(m_device->GetApiDevice(), m_pipeline_layout, nullptr);
	vkDestroyRenderPass(m_device->GetApiDevice(), m_render_pass, nullptr);

	for (size_t i = 0; i < m_swapchain_image_views.size(); ++i)
		vkDestroyImageView(m_device->GetApiDevice(), m_swapchain_image_views[i], nullptr);

	vkDestroySwapchainKHR(m_device->GetApiDevice(), m_swapchain, nullptr);
}

void VkContext::BindResourcesToPipeline()
{
	m_command_list->BindDescriptorSet(m_pipeline_layout, m_descriptor_sets[m_current_frame]);
}

void VkContext::Begin()
{
	m_command_list = m_command_queue->GetApiList(m_current_frame);

	this->CreateGraphicsPipeline();
	this->CreateDescriptorSets();

	m_command_queue->WaitForFence(m_current_frame);

	uint32_t image_index;
	VkResult result = m_command_list->AcquireNextImage(m_swapchain, image_index);

	VkCommandBufferBeginInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	m_command_list->Begin(info);

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

	m_command_list->BeginRenderPass(render_pass_info);
	m_command_list->BindPipeline(m_graphics_pipeline);
}

void VkContext::Present()
{
}

void VkContext::End()
{
	VkResult result = VK_SUCCESS;

	m_command_list->EndRenderPass();
	m_command_list->End();

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapchain();
		return;
	}

	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		ASSERT(false, "failed to acquire swap chain image!");

	UpdateUniformBuffer(m_current_frame);

	m_command_queue->Submit(m_command_list, m_current_frame);
	m_command_queue->Present(m_swapchain, m_command_list->GetRenderFinishedSemaphore(), m_current_frame);
	m_current_frame = (m_current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}

Context::API VkContext::GetApiType() const
{
	return API::VULKAN;
}

VkDevice VkContext::GetDevice() const
{
	return m_device->GetApiDevice();
}

VkCommandBuffer VkContext::GetCurrentCommandBuffer() const
{
	return m_command_list->GetApiBuffer();
}

VkInstance VkContext::GetInstance() const
{
	return m_instance;
}

VkPhysicalDevice VkContext::GetSelectedGpu() const
{
	return m_gpu->GetPhysicalDevice();
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

	unsigned int min_score = 0;
	for (const VkPhysicalDevice& gpu : gpus)
	{
		VkPhysicalDeviceProperties gpu_props;
		vkGetPhysicalDeviceProperties(gpu, &gpu_props);
		spdlog::info("checking {}", gpu_props.deviceName);

		VkGpu vk_gpu(gpu, m_surface);

		if (vk_gpu.IsSuitable())
		{
			if (vk_gpu.GetDeviceScore() > min_score)
			{
				m_gpu.reset(new VkGpu(gpu, m_surface));
				m_msaa_samples = GetMaxUsableSampleCount();
				spdlog::info("using: {}", gpu_props.deviceName);
				min_score = vk_gpu.GetDeviceScore();
			}
		}
	}

	ASSERT(m_gpu, "failed to find suitable gpu!");
}

void VkContext::CreateSurface(Window* window)
{
	VKCALL(vk_utils::CreateWindowSurface(m_instance, window, nullptr, &m_surface));
}

void VkContext::CreateLogicalDevice()
{
	m_device = std::make_unique<VkDefaultDevice>(m_gpu.get());
	VkCommandList::CreatePool(m_gpu->GetGraphicsPresentQueueIndex());
	m_command_queue = std::make_unique<VkCommandQueue>(m_gpu->GetGraphicsPresentQueueIndex(), MAX_FRAMES_IN_FLIGHT);
	m_command_list = m_command_queue->GetApiList(m_current_frame);
}

void VkContext::CreateSwapchain()
{
	SwapChainSupportDetails swapchain_support = m_gpu->GetSwapchainSupportDetails();// QuerySwapchainSupport(m_physical_device);

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

	//QueueFamilyIndices indices = FindQueueFamilies(m_physical_device);
	uint32_t queue_family_indices[] = { m_gpu->GetGraphicsPresentQueueIndex(), m_gpu->GetGraphicsPresentQueueIndex() };

	//if (indices.graphics_familiy != indices.present_family)
	//{
	//	create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
	//	create_info.queueFamilyIndexCount = 2;
	//	create_info.pQueueFamilyIndices = queue_family_indices;
	//}
	//else
	//{
		create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		create_info.queueFamilyIndexCount = 0;
		create_info.pQueueFamilyIndices = nullptr;
	//}

	create_info.preTransform = swapchain_support.capabilities.currentTransform;
	create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	create_info.presentMode = present_mode;
	create_info.clipped = VK_TRUE;

	create_info.oldSwapchain = VK_NULL_HANDLE;

	VKCALL(vkCreateSwapchainKHR(m_device->GetApiDevice(), &create_info, nullptr, &m_swapchain));

	vkGetSwapchainImagesKHR(m_device->GetApiDevice(), m_swapchain, &image_count, nullptr);
	m_swapchain_images.resize(image_count);
	vkGetSwapchainImagesKHR(m_device->GetApiDevice(), m_swapchain, &image_count, m_swapchain_images.data());

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

	VKCALL(vkCreateRenderPass(m_device->GetApiDevice(), &render_pass_info, nullptr, &m_render_pass));
	VKCALL(vkCreateRenderPass(m_device->GetApiDevice(), &render_pass_info, nullptr, &m_imgui_render_pass));

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

	VKCALL(vkCreateDescriptorSetLayout(m_device->GetApiDevice(), &layout_info, nullptr, &m_descriptor_set_layout));
}

void VkContext::CreateGraphicsPipeline()
{
	if (m_graphics_pipeline)
		return;

	VkPipelineInputAssemblyStateCreateInfo input_assembly = {};
	input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly.topology = m_bound_index_buffer->GetTopology().ToVulkan();
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

	VKCALL(vkCreatePipelineLayout(m_device->GetApiDevice(), &pipeline_layout_info, nullptr, &m_pipeline_layout));

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
	pipeline_info.pRasterizationState = &m_rasterizer_settings.ToVulkan();
	pipeline_info.pMultisampleState = &multi_sampling;
	pipeline_info.pDepthStencilState = &depth_stencil;
	pipeline_info.pColorBlendState = &color_blending;
	pipeline_info.pDynamicState = nullptr;

	pipeline_info.layout = m_pipeline_layout;
	pipeline_info.renderPass = m_render_pass;
	pipeline_info.subpass = 0;
	pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
	pipeline_info.basePipelineIndex = -1;

	VKCALL(vkCreateGraphicsPipelines(m_device->GetApiDevice(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_graphics_pipeline));
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

		VKCALL(vkCreateFramebuffer(m_device->GetApiDevice(), &framebuffer_info, nullptr, &m_swapchain_frame_buffers[i]));
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

	VKCALL(vkCreateDescriptorPool(m_device->GetApiDevice(), &pool_info, nullptr, &m_descriptor_pool));

	VkDescriptorPoolSize pool_sizes_arr[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, MAX_FRAMES_IN_FLIGHT },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_FRAMES_IN_FLIGHT },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, MAX_FRAMES_IN_FLIGHT },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, MAX_FRAMES_IN_FLIGHT },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, MAX_FRAMES_IN_FLIGHT },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, MAX_FRAMES_IN_FLIGHT },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_FRAMES_IN_FLIGHT },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, MAX_FRAMES_IN_FLIGHT },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, MAX_FRAMES_IN_FLIGHT },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, MAX_FRAMES_IN_FLIGHT },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, MAX_FRAMES_IN_FLIGHT }
	};

	pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = MAX_FRAMES_IN_FLIGHT * IM_ARRAYSIZE(pool_sizes_arr);
	pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes_arr);
	pool_info.pPoolSizes = pool_sizes_arr;
	VKCALL(vkCreateDescriptorPool(m_device->GetApiDevice(), &pool_info, nullptr, &m_imgui_descriptor_pool));
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

	VKCALL(vkAllocateDescriptorSets(m_device->GetApiDevice(), &alloc_info, &m_descriptor_sets[0]));

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

		vkUpdateDescriptorSets(m_device->GetApiDevice(), static_cast<uint32_t>(descriptor_writes.size()), descriptor_writes.data(), 0, nullptr);
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

	VKCALL(vkCreateImage(m_device->GetApiDevice(), &image_info, nullptr, &image));

	VkMemoryRequirements mem_requirements;
	vkGetImageMemoryRequirements(m_device->GetApiDevice(), image, &mem_requirements);

	VkMemoryAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	alloc_info.allocationSize = mem_requirements.size;
	alloc_info.memoryTypeIndex = FindMemoryType(mem_requirements.memoryTypeBits, properties);

	VKCALL(vkAllocateMemory(m_device->GetApiDevice(), &alloc_info, nullptr, &imageMemory));

	vkBindImageMemory(m_device->GetApiDevice(), image, imageMemory, 0);
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

VkExtent2D VkContext::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
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
	VKCALL(vkCreateImageView(m_device->GetApiDevice(), &view_info, nullptr, &image_view));

	return image_view;
}

VkSampleCountFlagBits VkContext::GetMaxUsableSampleCount()
{
	VkPhysicalDeviceProperties physical_device_properties;
	vkGetPhysicalDeviceProperties(m_gpu->GetPhysicalDevice(), &physical_device_properties);

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

void VkContext::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels)
{
	std::unique_ptr<VkCommandList> command_list = BeginSingleTimeCommands();

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

	command_list->PipelineBarrier(source_stage, destination_stage, barrier);
	EndSingleTimeCommands(command_list);
}

//void VkContext::GenerateMipMaps(VkImage image, VkFormat format, int32_t texWidth, int32_t texHeight, uint32_t mipLevels)
//{
//	VkFormatProperties format_props;
//	vkGetPhysicalDeviceFormatProperties(m_physical_device, format, &format_props);
//
//	if (!(format_props.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
//		ThrowException("txture image format does not support linear blitting!");
//
//	std::unique_ptr<VkCommandList> command_list = BeginSingleTimeCommands();
//	VkCommandBuffer command_buffer = command_list->GetApiBuffer();
//
//	VkImageMemoryBarrier barrier = {};
//	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
//	barrier.image = image;
//	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
//	barrier.dstAccessMask = VK_QUEUE_FAMILY_IGNORED;
//	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//	barrier.subresourceRange.baseArrayLayer = 0;
//	barrier.subresourceRange.layerCount = 1;
//	barrier.subresourceRange.levelCount = 1;
//
//	int32_t mip_width = texWidth;
//	int32_t mip_height = texHeight;
//
//	for (uint32_t i = 1; i < mipLevels; ++i)
//	{
//		barrier.subresourceRange.baseMipLevel = i - 1;
//		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
//		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
//		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
//		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
//
//		vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
//			VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
//
//		VkImageBlit blit = {};
//		blit.srcOffsets[0] = { 0,0,0 };
//		blit.srcOffsets[1] = { mip_width, mip_height };
//		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//		blit.srcSubresource.mipLevel = i - 1;
//		blit.srcSubresource.baseArrayLayer = 0;
//		blit.srcSubresource.layerCount = 1;
//		blit.dstOffsets[0] = { 0,0,0 };
//		blit.srcOffsets[1] = { mip_width > 1 ? static_cast<int32_t>(mip_width * 0.5f) : 1, mip_height > 1 ? static_cast<int32_t>(mip_height * 0.5f) : 1, 1 };
//		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//		blit.dstSubresource.mipLevel = i;
//		blit.dstSubresource.baseArrayLayer = 0;
//		blit.dstSubresource.layerCount = 1;
//
//		vkCmdBlitImage(command_buffer,
//			image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
//			image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
//			1, &blit, VK_FILTER_LINEAR);
//
//		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
//		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
//		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
//
//		vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
//			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
//
//		if (mip_width > 1) mip_width /= 2;
//		if (mip_height > 1) mip_height /= 2;
//
//	}
//
//	barrier.subresourceRange.baseMipLevel = 0; // m_mip_levels - 1;
//	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
//	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
//	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
//
//	vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
//	EndSingleTimeCommands(command_list);
//}
//
//void VkContext::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
//{
//	std::unique_ptr<VkCommandList> command_list = BeginSingleTimeCommands();
//	VkBufferImageCopy region = {};
//
//	region.bufferOffset = 0;
//	region.bufferRowLength = 0;
//	region.bufferImageHeight = 0;
//	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//	region.imageSubresource.mipLevel = 0;
//	region.imageSubresource.baseArrayLayer = 0;
//	region.imageSubresource.layerCount = 1;
//	region.imageOffset = { 0,0,0 };
//	region.imageExtent = { width, height, 1 };
//
//	vkCmdCopyBufferToImage(command_list->GetApiBuffer(), buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
//
//	EndSingleTimeCommands(command_list);
//}

uint32_t VkContext::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties mem_properties;
	vkGetPhysicalDeviceMemoryProperties(m_gpu->GetPhysicalDevice(), &mem_properties);

	for (uint32_t i = 0; i < mem_properties.memoryTypeCount; ++i)
	{
		if (typeFilter & (1 << i) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	}

	ThrowException("failed to find suitable memory type!");
}

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
		vkGetPhysicalDeviceFormatProperties(m_gpu->GetPhysicalDevice(), format, &props);

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

std::unique_ptr<VkCommandList> VkContext::BeginSingleTimeCommands()
{
	std::unique_ptr<VkCommandList> cmd_list = std::make_unique<VkCommandList>();
	
	VkCommandBufferBeginInfo begin_info = {};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	cmd_list->Begin(begin_info);

	return std::move(cmd_list);
}

void VkContext::EndSingleTimeCommands(std::unique_ptr<VkCommandList>& commandBuffer)
{
	commandBuffer->End();
	m_command_queue->DirectSubmit(commandBuffer->GetDirectSubmitInfo());
}

void VkContext::BindIndexBufferInternal(const IndexBuffer* indexBuffer)
{
	const VkIndexBuffer* vk_ib = static_cast<const VkIndexBuffer*>(indexBuffer);
	m_command_list->BindIndexBuffer(vk_ib->GetBufferGpu(), 0, vk_ib->GetFormat().ToVulkanIndexType());
}

void VkContext::UnbindIndexBufferInternal(const IndexBuffer* indexBuffer)
{
}

void VkContext::BindVertexArrayInternal(const VertexArray* vertexArray)
{
	const VkVertexBuffer* vk_vb = static_cast<const VkVertexBuffer*>(vertexArray->GetVertexBuffer());
	m_command_list->BindVertexBuffer(vk_vb->GetBufferGpu(), 0);
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
	vkDeviceWaitIdle(m_device->GetApiDevice());
	CleanupSwapchain();
	CreateSwapchain();
	CreateImageViews();
	CreateRenderPass();
	CreateGraphicsPipeline();
	CreateColorResources();
	CreateDepthResources();
	CreateFrameBuffers();
	//CreateCommandBuffers();
}

void VkContext::UpdateUniformBuffer(uint32_t imageIndex)
{
	m_bound_shader_program->UploadVariables();
}


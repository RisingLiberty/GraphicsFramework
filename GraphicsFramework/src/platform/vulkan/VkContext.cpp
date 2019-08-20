#include "stdafx.h"

#include "VkContext.h"
#include "VkHelperMethods.h"

#include "graphics/Window.h"

#include "VkVertexBuffer.h"
#include "VkIndexBuffer.h"

#include "VkShaderProgram.h"
#include "VkVertexLayout.h"
#include "VkVertexArray.h"

#include "VkRenderer.h"
#include "VkCommandQueue.h"
#include "VkCommandList.h"
#include "VkCommandPoolWrapper.h"
#include "VkDirectCommandList.h"

#include "VkGpu.h"
#include "VkDefaultDevice.h"
#include "VkImageWrapper.h"
#include "VkImageViewWrapper.h"
#include "VkInstanceWrapper.h"
#include "VkSwapchain.h"
#include "VkFrameBuffer.h"

#include "VkDescriptorSetsWrapper.h"
#include "VkDescriptorSetLayoutWrapper.h"
#include "VkDescriptorPoolWrapper.h"

#include "VkRenderPassWrapper.h"

#include "VkGraphicsPipelineWrapper.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb/stb_image.h>

//Define to include the function bodies and avoid linker issues
//#ifndef TINYOBJLOADER_IMPLEMENTATION
//#define TINYOBJLOADER_IMPLEMENTATION
//#include <objreader/tiny_obj_loader.h>
//#endif

namespace
{
	const std::string MODEL_PATH = "data/meshes/chalet.obj";
	const std::string TEXTURE_PATH = "data/textures/chalet.jpg";

	VkPhysicalDevice PickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
	{
		uint32_t gpu_count = 0;
		vkEnumeratePhysicalDevices(instance, &gpu_count, nullptr);

		ASSERT(gpu_count, "failed to find gpus with Vulkan support!");

		std::vector<VkPhysicalDevice> gpus(gpu_count);
		vkEnumeratePhysicalDevices(instance, &gpu_count, gpus.data());

		VkPhysicalDevice picked_device = VK_NULL_HANDLE;

		unsigned int min_score = 0;
		for (const VkPhysicalDevice& gpu : gpus)
		{
			VkPhysicalDeviceProperties gpu_props;
			vkGetPhysicalDeviceProperties(gpu, &gpu_props);
			spdlog::info("checking {}", gpu_props.deviceName);

			VkGpu vk_gpu(gpu, surface);

			if (vk_gpu.IsSuitable())
			{
				if (vk_gpu.GetDeviceScore() > min_score)
				{
					picked_device = gpu;
					spdlog::info("using: {}", gpu_props.deviceName);
					min_score = vk_gpu.GetDeviceScore();
				}
			}
		}

		ASSERT(picked_device, "failed to find suitable gpu!");

		return picked_device;
	}

	struct VkAllocatorMeta
	{
		bool shouldAllocate;
	};

	void* VkAllocate(void* userData, size_t size, size_t allignment, VkSystemAllocationScope allocationScope)
	{

		spdlog::info("VkAllocate");
		spdlog::info("allocating {} bytes with an allignment of {}", size, allignment);

		switch (allocationScope)
		{
		case VK_SYSTEM_ALLOCATION_SCOPE_COMMAND:
			spdlog::info("COMMAND");
			break;
		case VK_SYSTEM_ALLOCATION_SCOPE_OBJECT:
			spdlog::info("OBJECT");
			break;
		case VK_SYSTEM_ALLOCATION_SCOPE_CACHE:
			spdlog::info("CACHE");
			break;
		case VK_SYSTEM_ALLOCATION_SCOPE_DEVICE:
			spdlog::info("DEVICE");
			break;
		case VK_SYSTEM_ALLOCATION_SCOPE_INSTANCE:
			spdlog::info("INSTANCE");
			break;
		}

#ifdef _DEBUG
		return _aligned_malloc_dbg(size, allignment, __FILE__, __LINE__);
#else
		return _aligned_malloc(size, allignment);
#endif
	}
	void* VkReallocate(void* userData, void* pOriginal, size_t size, size_t allignment, VkSystemAllocationScope allocationScope)
	{

		spdlog::info("VkReallocate");
#ifdef _DEBUG
		return _aligned_realloc_dbg(pOriginal, size, allignment, __FILE__, __LINE__);
#else
		return realloc(pOriginal, size);
#endif
	}
	void VkFree(void* userData, void* memory)
	{
		spdlog::info("VkFree");
#ifdef _DEBUG
		_aligned_free_dbg(memory);
#else
		_aligned_free(memory);
#endif
	}
	void VkInternalAllocationNotification(void* userData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope)
	{
		spdlog::info("internal allocation notification");
	}
	void VkInternalFreeNotification(void* userData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope)
	{
		spdlog::info("internal free notification");
	}
}

void CheckVkResult(VkResult err)
{
	VKCALL(err);
}

std::unique_ptr<VkAllocationCallbacks> VkContext::s_allocator;

VkContext::VkContext(Window* window) :
	m_window(window)
{
	spdlog::info("Using Vulkan");

	static VkAllocatorMeta allocator_meta;
	allocator_meta.shouldAllocate = false;

	if (allocator_meta.shouldAllocate)
	{
		s_allocator = std::make_unique<VkAllocationCallbacks>();
		s_allocator->pfnAllocation = VkAllocate;
		s_allocator->pfnReallocation = VkReallocate;
		s_allocator->pfnFree = VkFree;
		s_allocator->pfnInternalAllocation = VkInternalAllocationNotification;
		s_allocator->pfnInternalFree = VkInternalFreeNotification;
	}

	m_instance = std::make_unique<VkInstanceWrapper>(window);
	this->ShowExtentions();
	m_surface = m_instance->CreateSurface(window);

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
	m_gpu = std::make_unique<VkGpu>(::PickPhysicalDevice(m_instance->GetApiInstance(), m_surface), m_surface);
	m_device = std::make_unique<VkDefaultDevice>(m_gpu.get());
	m_command_queue = std::make_unique<VkCommandQueue>(m_gpu->GetGraphicsPresentQueueIndex(), MAX_NR_OF_FRAMES_IN_FLIGHT);
	m_command_list = m_command_queue->As<VkCommandQueue>()->GetApiList(m_current_frame);

	//CreateGLFWWindow(m_window);
	//CreateTestContext(m_window);

	m_swapchain = std::make_unique<VkSwapchain>(m_gpu->GetSwapchainSupportDetails(), m_back_buffer_format.ToVulkan(), m_surface, m_gpu->GetGraphicsPresentQueueIndex());

	std::vector<VkImage> images = m_swapchain->GetImages();
	m_swapchain_image_views.resize(MAX_NR_OF_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < images.size(); ++i)
		m_swapchain_image_views[i] = std::make_unique<VkImageViewWrapper>(images[i], m_back_buffer_format.ToVulkan(), VK_IMAGE_ASPECT_COLOR_BIT, 1);

	m_render_pass = std::make_unique<VkRenderPassWrapper>(m_back_buffer_format);
	m_imgui_render_pass = std::make_unique<VkRenderPassWrapper>(m_back_buffer_format);
	m_descriptor_set_layout = std::make_unique<VkDescriptorSetLayoutWrapper>();

	//this->CreateColorResources();
	//this->CreateDepthResources();

	m_swapchain_frame_buffers.resize(MAX_NR_OF_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_NR_OF_FRAMES_IN_FLIGHT; ++i)
	{
		//std::array<VkImageView, 3> attachments =
		//{
		//	m_color_image_view_wrapper->GetApiImageView(),
		//	m_depth_image_view_wrapper->GetApiImageView(),
		//	m_swapchain_image_views[i]->GetApiImageView()
		//};

		std::vector<VkImageView> attachments =
		{
			m_swapchain_image_views[i]->GetApiImageView()
		};

		m_swapchain_frame_buffers[i] = std::make_unique<VkFrameBufferWrapper>(m_swapchain->GetExtent().width, m_swapchain->GetExtent().height, attachments, m_render_pass->GetApiRenderPass());
	}

	//this->CreateUniformBuffer();
	this->CreateDescriptorPool();

	this->InitializeImGui();

	m_renderer = std::make_unique<VkRenderer>();
}

void VkContext::Cleanup()
{
	m_command_queue->As<VkCommandQueue>()->WaitTillIdle();
	m_command_queue->As<VkCommandQueue>()->WaitForFence(m_current_frame);
	Context::CleanUp();

	// Currently needs to be done, because this depends on VkDevice
	m_command_queue.reset();

	//vkDestroySampler(m_device, m_texture_sampler, GetVkAllocationCallbacks());
	//vkDestroyImageView(m_device, m_texture_image_view, GetVkAllocationCallbacks());
	//vkDestroyImage(m_device, m_texture_image, GetVkAllocationCallbacks());
	//vkFreeMemory(m_device, m_texture_image_memory, GetVkAllocationCallbacks());

	//vkDestroyDescriptorPool(m_device->GetApiDevice(), m_descriptor_pool, GetVkAllocationCallbacks());
	vkDestroySurfaceKHR(m_instance->GetApiInstance(), m_surface, GetVkAllocationCallbacks());
}

void VkContext::BindResourcesToPipeline()
{
	m_command_list->BindDescriptorSet(m_pipeline_wrapper->GetLayout(), m_descriptor_sets->GetApiDescriptorSets()[m_current_frame]);
}

void VkContext::Begin()
{
	m_command_list = m_command_queue->As<VkCommandQueue>()->GetApiList(m_current_frame);

	this->CreateGraphicsPipeline();
	this->CreateDescriptorSets();

	m_command_queue->As<VkCommandQueue>()->WaitForFence(m_current_frame);

	uint32_t image_index;
	if (m_command_list->AcquireNextImage(m_swapchain.get(), image_index) == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapchain();
		return;
	}
	//else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	//	ASSERT(false, "failed to acquire swap chain image!");

	VkCommandBufferBeginInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	m_command_list->Begin(info);

	VkRenderPassBeginInfo render_pass_info = {};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	render_pass_info.renderPass = m_render_pass->GetApiRenderPass();
	render_pass_info.framebuffer = m_swapchain_frame_buffers[image_index]->GetApiFrameBuffer();
	render_pass_info.renderArea.offset = { 0, 0 };
	render_pass_info.renderArea.extent = m_swapchain->GetExtent();
	render_pass_info.clearValueCount = 1;

	std::array<VkClearValue, 2> clear_values = {};
	clear_values[0].color = { 1.0f, 0.0f, 0.0f, 1.0f };
	clear_values[1].depthStencil = { 1.0f, 0 };

	render_pass_info.clearValueCount = static_cast<uint32_t>(clear_values.size());
	render_pass_info.pClearValues = clear_values.data();

	m_command_list->BeginRenderPass(render_pass_info);
	m_command_list->BindPipeline(m_pipeline_wrapper->GetPipeline());
}

void VkContext::Present()
{
	m_command_list->EndRenderPass();
	m_command_list->End();

	m_command_queue->As<VkCommandQueue>()->Submit(m_command_list, m_current_frame);
	m_command_queue->As<VkCommandQueue>()->Present(m_swapchain->GetSwapchain(), m_command_list->GetRenderFinishedSemaphore(), m_current_frame);
}

void VkContext::End()
{
	m_current_frame = (m_current_frame + 1) % MAX_NR_OF_FRAMES_IN_FLIGHT;
}

API VkContext::GetApiType() const
{
	return API::VULKAN;
}

VkDevice VkContext::GetDevice() const
{
	return m_device->GetApiDevice();
}

VkCommandList* VkContext::GetCurrentCommandList() const
{
	return m_command_list;
}

VkCommandBuffer VkContext::GetCurrentCommandBuffer() const
{
	return this->GetCurrentCommandList()->GetApiBuffer();
}

VkAllocationCallbacks* VkContext::GetAllocationCallbacks()
{
	return s_allocator.get();
}

VkPhysicalDevice VkContext::GetSelectedGpu() const
{
	return m_gpu->GetPhysicalDevice();
}

void VkContext::ShowExtentions()
{
	std::vector<VkExtensionProperties> extensions = m_instance->GetExtensions();
	spdlog::info("available extensions:");

	for (const VkExtensionProperties& extension : extensions)
		spdlog::info("\t{}", extension.extensionName);
}

void VkContext::CreateGraphicsPipeline()
{
	if (m_pipeline_wrapper)
		return;

	const VkVertexLayout* vk_layout = m_bound_vertex_array->GetVertexLayout()->As<VkVertexLayout>();
	const VkShaderProgram* vk_shader = m_bound_shader_program->As<VkShaderProgram>();

	m_pipeline_wrapper = std::make_unique<VkGraphicsPipelineWrapper>(m_swapchain->GetExtent().width, m_swapchain->GetExtent().height,
		m_swapchain->GetExtent(), m_gpu->GetMaxUsableSampleCount(), m_bound_index_buffer->GetTopology(), m_descriptor_set_layout->GetApiDescriptorSetLayout(),
		m_rasterizer_settings, vk_layout->GetCreateInfo(), vk_shader->GetPipelineShaderStageCreateInfos().data(), m_render_pass->GetApiRenderPass());

}

//void VkContext::CreateColorResources()
//{
//	VkFormat color_format = m_back_buffer_format.ToVulkan();
//
//	m_color_image_wrapper = std::make_unique<VkImageWrapper>(m_swapchain_extent.width, m_swapchain_extent.height,
//		m_msaa_samples, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
//		color_format, VK_IMAGE_ASPECT_COLOR_BIT, 1, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
//
//	m_color_image_wrapper->TransitionImageLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
//}
//
//void VkContext::CreateDepthResources()
//{
//	VkFormat depth_format = FindDepthFormat();
//	m_depth_image_wrapper = std::make_unique<VkImageWrapper>(
//		m_swapchain_extent.width, m_swapchain_extent.height,
//		m_msaa_samples, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
//		depth_format, VK_IMAGE_ASPECT_DEPTH_BIT, 1, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
//		);
//
//	m_depth_image_wrapper->TransitionImageLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
//}

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
//	memcpy(data, pixels, static_castt<size_t>(image_size));
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
//	vkDestroyBuffer(m_device, staging_buffer, GetVkAllocationCallbacks());
//	vkFreeMemory(m_device, staging_buffer_memory, GetVkAllocationCallbacks());
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
//	sampler_info.maxLod = static_castt<float>(m_mip_levels);
//
//	VKCALL(vkCreateSampler(m_device, &sampler_info, GetVkAllocationCallbacks(), &m_texture_sampler));
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

//void VkContext::CreateUniformBuffer()
//{
	//VkDeviceSize buffer_size = sizeof(UniformBufferObject);
	//
	//m_uniform_buffers.resize(m_swapchain_images.size());
	//m_uniform_buffers_memory.resize(m_swapchain_images.size());
	//
	//for (size_t i = 0; i < m_swapchain_images.size(); ++i)
	//{
	//	CreateBuffer(buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniform_buffers[i], m_uniform_buffers_memory[i]);
	//}
//}

void VkContext::CreateDescriptorPool()
{
	std::vector<VkDescriptorPoolSize> pool_sizes =
	{
		{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_NR_OF_FRAMES_IN_FLIGHT},
		{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_NR_OF_FRAMES_IN_FLIGHT}
	};

	m_descriptor_pool = std::make_unique<VkDescriptorPoolWrapper>(MAX_NR_OF_FRAMES_IN_FLIGHT, pool_sizes);

	std::vector<VkDescriptorPoolSize> pool_sizes_imgui =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, MAX_NR_OF_FRAMES_IN_FLIGHT },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_NR_OF_FRAMES_IN_FLIGHT },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, MAX_NR_OF_FRAMES_IN_FLIGHT },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, MAX_NR_OF_FRAMES_IN_FLIGHT },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, MAX_NR_OF_FRAMES_IN_FLIGHT },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, MAX_NR_OF_FRAMES_IN_FLIGHT },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_NR_OF_FRAMES_IN_FLIGHT },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, MAX_NR_OF_FRAMES_IN_FLIGHT },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, MAX_NR_OF_FRAMES_IN_FLIGHT },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, MAX_NR_OF_FRAMES_IN_FLIGHT },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, MAX_NR_OF_FRAMES_IN_FLIGHT }
	};

	m_imgui_descriptor_pool = std::make_unique<VkDescriptorPoolWrapper>(MAX_NR_OF_FRAMES_IN_FLIGHT * (unsigned int)pool_sizes_imgui.size(), pool_sizes_imgui);
}

void VkContext::CreateDescriptorSets()
{
	if (m_descriptor_sets)
		return;

	std::vector<VkDescriptorSetLayout> layouts(MAX_NR_OF_FRAMES_IN_FLIGHT, m_descriptor_set_layout->GetApiDescriptorSetLayout());
	m_descriptor_sets = std::make_unique<VkDescriptorSetsWrapper>(MAX_NR_OF_FRAMES_IN_FLIGHT, layouts, m_bound_shader_program->As<VkShaderProgram>()->GetUniformBuffer(), m_descriptor_pool->GetDescriptorPool());
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
//			VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, GetVkAllocationCallbacks(), 0, GetVkAllocationCallbacks(), 1, &barrier);
//
//		VkImageBlit blit = {};
//		blit.srcOffsets[0] = { 0,0,0 };
//		blit.srcOffsets[1] = { mip_width, mip_height };
//		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//		blit.srcSubresource.mipLevel = i - 1;
//		blit.srcSubresource.baseArrayLayer = 0;
//		blit.srcSubresource.layerCount = 1;
//		blit.dstOffsets[0] = { 0,0,0 };
//		blit.srcOffsets[1] = { mip_width > 1 ? static_castt<int32_t>(mip_width * 0.5f) : 1, mip_height > 1 ? static_castt<int32_t>(mip_height * 0.5f) : 1, 1 };
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
//			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, GetVkAllocationCallbacks(), 0, GetVkAllocationCallbacks(), 1, &barrier);
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
//	vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, GetVkAllocationCallbacks(), 0, GetVkAllocationCallbacks(), 1, &barrier);
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

std::unique_ptr<VkCommandList> VkContext::CreateDirectCommandList() const
{
	return m_command_queue->As<VkCommandQueue>()->CreateDirectCommandList();
}

void VkContext::InitializeImGui() const
{
	// Setup Platform/Renderer bindings
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = m_instance->GetApiInstance();
	init_info.PhysicalDevice = m_gpu->GetPhysicalDevice();
	init_info.Device = m_device->GetApiDevice();
	init_info.QueueFamily = m_gpu->GetGraphicsPresentQueueIndex();
	init_info.Queue = m_command_queue->As<VkCommandQueue>()->GetApiQueue();
	init_info.PipelineCache = nullptr;
	init_info.DescriptorPool = m_imgui_descriptor_pool->GetDescriptorPool();
	init_info.Allocator = GetVkAllocationCallbacks();
	init_info.MinImageCount = MAX_NR_OF_FRAMES_IN_FLIGHT;
	init_info.ImageCount = MAX_NR_OF_FRAMES_IN_FLIGHT; //  m_vulkan_window->ImageCount;
	init_info.CheckVkResultFn = CheckVkResult;
	ImGui_ImplVulkan_Init(&init_info, m_imgui_render_pass->GetApiRenderPass());

	// Load Fonts
	std::unique_ptr<VkCommandList> command_buffer = this->CreateDirectCommandList();
	ImGui_ImplVulkan_CreateFontsTexture(command_buffer->GetApiBuffer());
	ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void VkContext::BindIndexBufferInternal(const IndexBuffer* indexBuffer)
{
	const VkIndexBuffer* vk_ib = indexBuffer->As<VkIndexBuffer>();
	m_command_list->BindIndexBuffer(vk_ib->GetBufferGpu(), 0, vk_ib->GetFormat().ToVulkanIndexType());
}

void VkContext::UnbindIndexBufferInternal(const IndexBuffer* indexBuffer)
{
}

void VkContext::BindVertexArrayInternal(const VertexArray* vertexArray)
{
	const VkVertexBuffer* vk_vb = vertexArray->GetVertexBuffer()->As<VkVertexBuffer>();
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
	//CleanupSwapchain();
	//CreateSwapchain();
	//CreateImageViews();
	//CreateRenderPass();
	CreateGraphicsPipeline();
	//CreateColorResources();
	//CreateDepthResources();
	//CreateFrameBuffers();
	//CreateCommandBuffers();
}


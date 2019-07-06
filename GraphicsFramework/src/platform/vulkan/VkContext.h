#pragma once

#include "graphics/Context.h"

class Window;

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <glm/gtc/matrix_transform.hpp>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec2 TexCoord;

	static VkVertexInputBindingDescription GetBindingDescription()
	{
		//A vertex binding describes at which rate to load data from memory throught the vertices.
		//It specifies the number of bytes between data entries and wheter to move to the next data entry
		//after each vertex or after each instance.
		VkVertexInputBindingDescription bindingDescription = {};

		//all of our per-vertex data is packed together in one array, so we're only going to have one binding.
		//the binding paramter specifies the index of the binding in the array of bindings.
		//The stride parameters specifies the number of bytes from one entry to the next
		//the inputRate parameter can have one of the following values:
		//VK_VERTEX_INPUT_RATE_VERTEX: move to the next data entry after each vertex
		//VK_VERTEX_INPUT_RATE_INSTANCE: move to the next data entry after each instance.
		//we're not going to use instanced rendering, so we'll stick to per-vertex data.
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions()
	{
		//The second structure that describes how to handle vertex input is VkVertexInputAttributeDescription.
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

		//An attribute description struct describes how to extract a vertex attribute from a chuknk of vertex data.
		//originating from a binding description.
		//We have 2 attributes, position and color, so we need 2 attribute description structs

		//POSITION
		//------------------

		//Tells Vulkan from which binding the per-vertex data comes
		attributeDescriptions[0].binding = 0;
		//The location parameter references the location directive of the input in the vertex shader
		//The input in the vertex shader with location 0 is the position, which has 2 32-bit float components
		attributeDescriptions[0].location = 0;
		//The format parameter describes the type of data for the attribute.
		//A bit confusingly, the formata are specified using the same enumeration as color formats.
		//The following shader types and formats are commonly used together:
		//float: VK_FORMAT_R32_SFLOAT
		//vec2: VK_FORMAT_R32G32_SFLOAT
		//vec3: VK_FORMAT_R32G32B32_SFLOAT
		//vec4: VK_FORMAT_R32G32B32A32_SFLOAT
		//The color type (SFLOAT_, UINT, SINT) and bit width should also match the type of the shader input.
		//Examples:
		//ivec2: VK_FORMAT_R32G32_SINT, a 2-component vector of 32-bit signed integers
		//uvec4: VK_FORMAT_R32G32B32A32, a 4-component vector of 32-bit unsigned integers
		//double: VK_FORMAT_R64_SFLOAT, a double-precision (64-bit) float
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		//specifies the number of bytes since the start of the per-vertex data to read from.
		//The bniding is loading one Vertex at a time and the position attribute(Position) i at an offset of 0 bytes
		//from the beginning of this struct. This is automatically calculated using the offsetof macro.
		attributeDescriptions[0].offset = offsetof(Vertex, Position);

		//COLOR
		//-----------------
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, Color);

		//TEXCOORD
		//-----------------
		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, TexCoord);

		return attributeDescriptions;
	}

	bool operator==(const Vertex& other) const
	{
		return Position == other.Position && Color == other.Color && TexCoord == other.TexCoord;
	}

};

namespace std
{
	template<> struct hash<Vertex>
	{
		size_t operator()(Vertex const& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.Position) ^
				(hash<glm::vec3>()(vertex.Color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.TexCoord) << 1);
		}
	};
}



class VkContext : public Context
{
public:
	VkContext(Window* window);
	~VkContext();

	virtual void Initialize();
	virtual void Present();
	virtual API GetApiType() const;

private:
	void CreateInstance(Window* window);
	void ShowExtentions();
	void SetupDebugCallback();
	void PickPhysicalDevice();
	void CreateLogicalDevice();
	void CreateSwapchain();
	void CreateImageViews();
	void CreateRenderPass();
	void CreateDescriptorSetLayout();
	void CreateGraphicsPipeline();
	void CreateCommandPool();
	void CreateColorResources();
	void CreateDepthResources();
	void CreateFrameBuffers();
	void CreateTextureImage();
	void CreateTextureImageView();
	void CreateTextureSampler();
	void LoadModel();
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreateUniformBuffer();
	void CreateDescriptorPool();
	void CreateDescriptorSets();
	void CreateCommandBuffers();
	void CreateSyncObjects();

	void Cleanup();
	void CleanupSwapchain();

	bool CheckValidationLayerSupport() const;
	std::vector<const char*> GetRequiredExtentions() const;
private:
	VkInstance m_instance;
	VkDebugUtilsMessengerEXT m_debug_callback;
	VkPhysicalDevice* m_physical_device;
	VkSurfaceKHR m_surface;
	VkDevice m_device;
	VkQueue m_graphics_queue;
	VkQueue m_present_queue;
	VkSwapchainKHR m_swapchain;
	std::vector<VkImage> m_swapchain_images;
	VkFormat m_swapchain_image_format;
	VkExtent2D m_swapchain_extent;
	std::vector<VkImageView> m_swapchain_image_views;
	VkRenderPass m_render_pass;
	VkPipelineLayout m_pipeline_layout;
	VkPipeline m_graphics_pipeline;
	std::vector<VkFramebuffer> m_swapchain_frame_buffers;
	VkCommandPool m_command_pool;
	std::vector<VkCommandBuffer> m_command_buffers;
	std::vector<VkSemaphore> m_image_available_semaphores;
	std::vector<VkSemaphore> m_render_finished_semaphores;
	std::vector<VkFence> m_in_flight_fences;
	size_t m_current_frame;
	bool m_is_frame_buffer_resized;
	VkBuffer m_vertex_buffer;
	VkDeviceMemory m_vertex_buffer_memory;
	VkBuffer m_index_buffer;
	VkDeviceMemory m_index_buffer_memory;
	VkDescriptorSetLayout m_descriptor_set_layout;
	std::vector<VkBuffer> m_uniform_buffers;
	std::vector<VkDeviceMemory> m_uniform_buffers_memory;
	VkDescriptorPool m_descriptor_pool;
	std::vector<VkDescriptorSet> m_descriptor_sets;
	uint32_t m_mip_levels;
	VkImage m_texture_image;
	VkDeviceMemory m_texture_image_memory;
	VkImageView m_texture_image_view;
	VkSampler m_texture_sampler;
	VkImage m_depth_image;
	VkDeviceMemory m_depth_image_memory;
	VkImageView m_depth_image_view;
	VkSampleCountFlagBits m_msaa_samples;
	VkImage m_color_image;
	VkDeviceMemory m_color_image_memory;
	VkImageView m_color_image_view;
	const std::vector<const char*> m_validation_layers = { "VK_LYER_LUNARG_standard_validation" };
	const std::vector<const char*> m_device_extentions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	const int MAX_FRAMES_IN_FLIGHT = 2;

	std::vector<Vertex> m_Vertices =
	{
		//		Position				Color			TexCoord
		//-----------------------------------------------------------
		{{-0.5f, -0.5f,  0.0f},  {1.0f, 0.0f, 0.0f},  {1.0f, 0.0f}},
		{{ 0.5f, -0.5f,  0.0f},  {0.0f, 1.0f, 0.0f},  {0.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.0f},  {0.0f, 0.0f, 1.0f},  {0.0f, 1.0f}},
		{{-0.5f,  0.5f,  0.0f},  {1.0f, 1.0f, 1.0f},  {1.0f, 1.0f}}
	};
	
	std::vector<uint32_t> m_Indices =
	{
		//First plane
		0,1,2, //top right
		2,3,0,  //bottom left
	};
	
	bool m_enable_validation_layers = true;

	const std::string MODEL_PATH = "data/meshes/chalet.obj";
	const std::string TEXTURE_PATH = "data/textures/chalet.obj";
};
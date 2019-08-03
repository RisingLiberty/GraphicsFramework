#pragma once

#include "graphics/Context.h"

class Window;
struct Imgui_ImplVulkanH_Window;

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <glm/gtc/matrix_transform.hpp>

class VkVertexBuffer;
class VkIndexBuffer;
class VkShaderProgram;
class VkVertexLayout;
class VertexArray;
class VkVertexArray;

class VkCommandQueue;
class VkCommandList;
class VkGpu;
class VkDefaultDevice;
class VkImageWrapper;
class VkImageViewWrapper;
class VkInstanceWrapper;
class VkSwapchain;

struct Vertex
{
	glm::vec3 Position;
	/*glm::vec3 Color;
	glm::vec2 TexCoord;
*/
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

	static std::array<VkVertexInputAttributeDescription, 1> GetAttributeDescriptions()
	{
		//The second structure that describes how to handle vertex input is VkVertexInputAttributeDescription.
		std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions = {};

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
		//attributeDescriptions[1].binding = 0;
		//attributeDescriptions[1].location = 1;
		//attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		//attributeDescriptions[1].offset = offsetof(Vertex, Color);

		////TEXCOORD
		////-----------------
		//attributeDescriptions[2].binding = 0;
		//attributeDescriptions[2].location = 2;
		//attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		//attributeDescriptions[2].offset = offsetof(Vertex, TexCoord);

		return attributeDescriptions;
	}

	bool operator==(const Vertex& other) const
	{
		return Position == other.Position;// && Color == other.Color && TexCoord == other.TexCoord;
	}

};

namespace std
{
	template<> struct hash<Vertex>
	{
		size_t operator()(Vertex const& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.Position)));/* ^
				(hash<glm::vec3>()(vertex.Color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.TexCoord) << 1);*/
		}
	};
}

struct QueueFamilyIndices
{
	int graphics_familiy = -1;
	int present_family = -1;

	bool IsComplete() { return graphics_familiy >= 0 && present_family >= 0; }
};

struct UniformBufferObject
{
	glm::vec4 color;
};

class VkContext : public Context
{
public:
	VkContext(Window* window);
	virtual ~VkContext();

	virtual void Initialize();
	virtual void Begin() override;
	virtual void Present();
	virtual void End() override;
	virtual API GetApiType() const;

	void BindResourcesToPipeline();

	VkDevice GetDevice() const;
	VkPhysicalDevice GetSelectedGpu() const;
	VkCommandList* GetCurrentCommandList() const;
	VkCommandBuffer GetCurrentCommandBuffer() const;

	std::unique_ptr<VkCommandList> BeginSingleTimeCommands();
	void EndSingleTimeCommands(std::unique_ptr<VkCommandList>& commandBuffer);

protected:
	void BindIndexBufferInternal(const IndexBuffer* indexBuffer) override;
	void UnbindIndexBufferInternal(const IndexBuffer* indexBuffer) override;

	void BindVertexArrayInternal(const VertexArray* vertexArray) override;
	void UnbindVertexArrayInternal(const VertexArray* vertexArray) override;

	void BindShaderProgramInternal(const ShaderProgram* shaderProgram) override;
	void UnbindShaderProgramInternal(const ShaderProgram* shaderProgram) override;

private:
	void ShowExtentions();
	void PickPhysicalDevice();
	void CreateRenderPass();
	void CreateDescriptorSetLayout();
	void CreateGraphicsPipeline();
	//void CreateColorResources();
	//void CreateDepthResources();
	void CreateFrameBuffers();
	void CreateTextureImage();
	void CreateTextureImageView();
	void CreateTextureSampler();
	//void LoadModel();
	void CreateUniformBuffer();
	void CreateDescriptorPool();
	void CreateDescriptorSets();

	void Cleanup();
	void CleanupSwapchain();

private:
	//void GenerateMipMaps(VkImage image, VkFormat format, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
	//void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void RecreateSwapchain();
	void UpdateUniformBuffer(uint32_t imageIndex);

private:
	VkSurfaceKHR m_surface;

	VkRenderPass m_imgui_render_pass;
	VkRenderPass m_render_pass;
	VkPipelineLayout m_pipeline_layout = {};
	VkPipeline m_graphics_pipeline;
	VkDescriptorSetLayout m_descriptor_set_layout;
	VkDescriptorPool m_descriptor_pool;
	VkDescriptorPool m_imgui_descriptor_pool;
	std::vector<VkDescriptorSet> m_descriptor_sets;
	std::vector<VkFramebuffer> m_swapchain_frame_buffers;

	//std::vector<VkBuffer> m_uniform_buffers;
	//std::vector<VkDeviceMemory> m_uniform_buffers_memory;

	uint32_t m_current_frame = 0;
	bool m_is_frame_buffer_resized = false;

	std::unique_ptr<VkInstanceWrapper> m_instance;
	std::unique_ptr<VkDefaultDevice> m_device;
	std::unique_ptr<VkCommandQueue> m_command_queue;
	VkCommandList* m_command_list;
	std::unique_ptr<VkGpu> m_gpu;
	//std::unique_ptr<VkImageWrapper> m_depth_image_wrapper;
	//std::unique_ptr<VkImageViewWrapper> m_depth_image_view_wrapper;
	//std::unique_ptr<VkImageWrapper> m_color_image_wrapper;
	//std::unique_ptr<VkImageViewWrapper> m_color_image_view_wrapper;
	std::vector<std::unique_ptr<VkImageWrapper>> m_swapchain_images;
	std::vector<std::unique_ptr<VkImageViewWrapper>> m_swapchain_image_views;
	std::unique_ptr<VkSwapchain> m_swapchain;
};
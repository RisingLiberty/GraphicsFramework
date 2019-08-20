#pragma once

#include "graphics/Context.h"

class Window;
struct Imgui_ImplVulkanH_Window;

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
class VkFrameBufferWrapper;
class VkDescriptorSetsWrapper;
class VkDescriptorSetLayoutWrapper;
class VkDescriptorPoolWrapper;
class VkRenderPassWrapper;
class VkGraphicsPipelineWrapper;

//struct Vertex
//{
//	glm::vec3 Position;
//	/*glm::vec3 Color;
//	glm::vec2 TexCoord;
//*/
//	static VkVertexInputBindingDescription GetBindingDescription()
//	{
//		//A vertex binding describes at which rate to load data from memory throught the vertices.
//		//It specifies the number of bytes between data entries and wheter to move to the next data entry
//		//after each vertex or after each instance.
//		VkVertexInputBindingDescription bindingDescription = {};
//
//		//all of our per-vertex data is packed together in one array, so we're only going to have one binding.
//		//the binding paramter specifies the index of the binding in the array of bindings.
//		//The stride parameters specifies the number of bytes from one entry to the next
//		//the inputRate parameter can have one of the following values:
//		//VK_VERTEX_INPUT_RATE_VERTEX: move to the next data entry after each vertex
//		//VK_VERTEX_INPUT_RATE_INSTANCE: move to the next data entry after each instance.
//		//we're not going to use instanced rendering, so we'll stick to per-vertex data.
//		bindingDescription.binding = 0;
//		bindingDescription.stride = sizeof(Vertex);
//		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
//
//		return bindingDescription;
//	}
//
//	static std::array<VkVertexInputAttributeDescription, 1> GetAttributeDescriptions()
//	{
//		//The second structure that describes how to handle vertex input is VkVertexInputAttributeDescription.
//		std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions = {};
//
//		//An attribute description struct describes how to extract a vertex attribute from a chuknk of vertex data.
//		//originating from a binding description.
//		//We have 2 attributes, position and color, so we need 2 attribute description structs
//
//		//POSITION
//		//------------------
//
//		//Tells Vulkan from which binding the per-vertex data comes
//		attributeDescriptions[0].binding = 0;
//		//The location parameter references the location directive of the input in the vertex shader
//		//The input in the vertex shader with location 0 is the position, which has 2 32-bit float components
//		attributeDescriptions[0].location = 0;
//		//The format parameter describes the type of data for the attribute.
//		//A bit confusingly, the formata are specified using the same enumeration as color formats.
//		//The following shader types and formats are commonly used together:
//		//float: VK_FORMAT_R32_SFLOAT
//		//vec2: VK_FORMAT_R32G32_SFLOAT
//		//vec3: VK_FORMAT_R32G32B32_SFLOAT
//		//vec4: VK_FORMAT_R32G32B32A32_SFLOAT
//		//The color type (SFLOAT_, UINT, SINT) and bit width should also match the type of the shader input.
//		//Examples:
//		//ivec2: VK_FORMAT_R32G32_SINT, a 2-component vector of 32-bit signed integers
//		//uvec4: VK_FORMAT_R32G32B32A32, a 4-component vector of 32-bit unsigned integers
//		//double: VK_FORMAT_R64_SFLOAT, a double-precision (64-bit) float
//		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
//		//specifies the number of bytes since the start of the per-vertex data to read from.
//		//The bniding is loading one Vertex at a time and the position attribute(Position) i at an offset of 0 bytes
//		//from the beginning of this struct. This is automatically calculated using the offsetof macro.
//		attributeDescriptions[0].offset = offsetof(Vertex, Position);
//
//		//COLOR
//		//-----------------
//		//attributeDescriptions[1].binding = 0;
//		//attributeDescriptions[1].location = 1;
//		//attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
//		//attributeDescriptions[1].offset = offsetof(Vertex, Color);
//
//		////TEXCOORD
//		////-----------------
//		//attributeDescriptions[2].binding = 0;
//		//attributeDescriptions[2].location = 2;
//		//attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
//		//attributeDescriptions[2].offset = offsetof(Vertex, TexCoord);
//
//		return attributeDescriptions;
//	}
//
//	bool operator==(const Vertex& other) const
//	{
//		return Position == other.Position;// && Color == other.Color && TexCoord == other.TexCoord;
//	}
//
//};
//
//namespace std
//{
//	template<> struct hash<Vertex>
//	{
//		size_t operator()(Vertex const& vertex) const
//		{
//			return ((hash<glm::vec3>()(vertex.Position)));/* ^
//				(hash<glm::vec3>()(vertex.Color) << 1)) >> 1) ^
//				(hash<glm::vec2>()(vertex.TexCoord) << 1);*/
//		}
//	};
//}

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
	static VkAllocationCallbacks* GetAllocationCallbacks();

	std::unique_ptr<VkCommandList> CreateDirectCommandList() const;

protected:
	void BindIndexBufferInternal(const IndexBuffer* indexBuffer) override;
	void UnbindIndexBufferInternal(const IndexBuffer* indexBuffer) override;

	void BindVertexArrayInternal(const VertexArray* vertexArray) override;
	void UnbindVertexArrayInternal(const VertexArray* vertexArray) override;

	void BindShaderProgramInternal(const ShaderProgram* shaderProgram) override;
	void UnbindShaderProgramInternal(const ShaderProgram* shaderProgram) override;

private:
	void ShowExtentions();
	void CreateGraphicsPipeline();
	void CreateDescriptorPool();
	void CreateDescriptorSets();

	void Cleanup();
	void InitializeImGui() const;

private:
	//void CreateColorResources();
	//void CreateDepthResources();
	//void CreateTextureImage();
	//void CreateTextureImageView();
	//void CreateTextureSampler();
	//void LoadModel();
	//void CreateUniformBuffer();
	//void GenerateMipMaps(VkImage image, VkFormat format, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
	//void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void RecreateSwapchain();

private:
	VkSurfaceKHR m_surface;

	uint32_t m_current_frame = 0;
	bool m_is_frame_buffer_resized = false;

	std::unique_ptr<VkInstanceWrapper> m_instance;
	std::unique_ptr<VkDefaultDevice> m_device;

	static std::unique_ptr<VkAllocationCallbacks> s_allocator;
	std::vector<std::unique_ptr<VkImageViewWrapper>> m_swapchain_image_views;
	std::vector<std::unique_ptr<VkFrameBufferWrapper>> m_swapchain_frame_buffers;

	//std::unique_ptr<VkCommandQueue> m_command_queue;
	std::unique_ptr<VkGpu> m_gpu;
	std::unique_ptr<VkSwapchain> m_swapchain;
	std::unique_ptr<VkDescriptorSetsWrapper> m_descriptor_sets;
	std::unique_ptr<VkDescriptorSetLayoutWrapper> m_descriptor_set_layout;
	std::unique_ptr<VkDescriptorPoolWrapper> m_descriptor_pool;
	std::unique_ptr<VkDescriptorPoolWrapper> m_imgui_descriptor_pool;
	std::unique_ptr<VkRenderPassWrapper> m_render_pass;
	std::unique_ptr<VkRenderPassWrapper> m_imgui_render_pass;

	std::unique_ptr<VkGraphicsPipelineWrapper> m_pipeline_wrapper;

	VkCommandList* m_command_list;

	Window* m_window;

	//std::vector<VkBuffer> m_uniform_buffers;
	//std::vector<VkDeviceMemory> m_uniform_buffers_memory;
	//std::unique_ptr<VkImageWrapper> m_depth_image_wrapper;
	//std::unique_ptr<VkImageViewWrapper> m_depth_image_view_wrapper;
	//std::unique_ptr<VkImageWrapper> m_color_image_wrapper;
	//std::unique_ptr<VkImageViewWrapper> m_color_image_view_wrapper;
};
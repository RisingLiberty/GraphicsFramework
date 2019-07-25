#pragma once

#include "graphics/Context.h"

class Window;
struct Imgui_ImplVulkanH_Window;

class VkTestContext : public Context
{
public:
	VkTestContext(Window* window);
	virtual ~VkTestContext();

	virtual void Initialize() override;

	virtual void Present() override;
	virtual API GetApiType() const override;

	void SetupVulkan(const char** extensions, uint32_t extensions_count);

private:
	void CreateInstance(const char** extensions, uint32_t extensions_count);
	void ShowExtentions();
	void SetupDebugCallback();
	void PickPhysicalDevice();
	void CreateSurface(Window* window);
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
	//void LoadModel();
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

	bool IsGpuSuitable(const VkPhysicalDevice& gpu);

private:
	Window* m_window;
	ImGui_ImplVulkanH_Window* m_wd;
};
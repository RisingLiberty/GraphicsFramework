#pragma once

#include "graphics/Context.h"

class Window;

class Dx11VertexShader;
class Dx11FragmentShader;

class Window;

class Dx11CommandQueue;
class Dx11Swapchain;
class Dx11CommandList;

class Dx11Context : public Context
{
public:

	Dx11Context(Window* window);
	virtual ~Dx11Context();

	void ResizeBuffers(unsigned int width, unsigned int height);

	virtual void PreInitialize() override;
	virtual void Initialize() override;
	virtual void Begin() override;
	virtual void Present() override;
	virtual API GetApiType() const override;

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;
	Dx11CommandList* GetCommandList() const;
	
	std::unique_ptr<Dx11CommandList> CreateDirectCommandList() const;

	const Dx11VertexShader* GetBoundVertexShader() const;

protected:
	void BindIndexBufferInternal(const IndexBuffer* indexBuffer) override;
	void UnbindIndexBufferInternal(const IndexBuffer* indexBuffer) override;

	void BindVertexArrayInternal(const VertexArray* vertexArray) override;
	void UnbindVertexArrayInternal(const VertexArray* vertexArray) override;

	void BindShaderProgramInternal(const ShaderProgram* shaderProgram) override;
	void UnbindShaderProgramInternal(const ShaderProgram* shaderProgram) override;

private:
	void InitD3D(Window* window);

private:

	D3D_FEATURE_LEVEL m_feature_level;

	ComPtr<ID3D11RenderTargetView> m_render_target_view;
	ComPtr<ID3D11DepthStencilView> m_depth_stencil_view;
	ComPtr<ID3D11Texture2D> m_depth_stencil_buffer;

	D3D11_VIEWPORT m_viewport;

	Window* m_window;

	//std::unique_ptr<Dx11CommandQueue> m_command_queue;
	//Dx11CommandList* m_command_list;
	std::unique_ptr<Dx11Swapchain> m_swapchain;
};
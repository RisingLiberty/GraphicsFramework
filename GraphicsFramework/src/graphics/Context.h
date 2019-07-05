#pragma once

class Renderer;
class ShaderController;
class VertexArrayController;
class Window;

class Context
{
public: 
	enum class API
	{
		DIRECTX11,
		DIRECTX12,
		OPENGL,
		VULKAN
	};

	Context();
	virtual ~Context();

	virtual void Initialize() = 0;

	static void Create(API api, Window* window);
	static API GetApi();
	static Context* GetCurrent();

	Renderer* GetRenderer() const;
	ShaderController* GetShaderController() const;
	VertexArrayController* GetVertexArrayController() const;

	virtual void Present() = 0;
	virtual API GetApiType() const = 0;

protected:

protected:
	std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<ShaderController> m_shader_controller;
	std::unique_ptr<VertexArrayController> m_vertex_array_controller;

private:
	static std::unique_ptr<Context> s_current;

};
#pragma once

class Renderer;
class ShaderController;
class VertexArrayController;
class VertexBufferController;
class VertexLayoutController;
class IndexBufferController;
class Window;

class IndexBuffer;
class VertexArray;
class ShaderProgram;
class CommandQueue;
class CommandList;

#include "RasterizerSettings.h"
#include "BufferHelpers.h"
#include "API.h"

class Context
{
public: 

	Context();
	virtual ~Context();

	virtual void PreInitialize();
	virtual void Initialize() = 0;
	virtual void PostInitialize();

	virtual void Begin();
	virtual void End();

	void Clear();

	void BindVertexArray(const VertexArray* va);
	void UnbindVertexArray(const VertexArray* va);
	
	void BindIndexBuffer(const IndexBuffer* indexBuffer);
	void UnbindIndexBuffer(const IndexBuffer* indexBuffer);

	void BindShaderProgram(const ShaderProgram* shaderProgram);
	void UnbindShaderProgram(const ShaderProgram* shaderProgram);

	static Context* Create(API api, Window* window);
	static Context* Switch(API api, Window* window);
	static API GetApi();
	static Context* GetCurrent();

	Renderer* GetRenderer() const;
	ShaderController* GetShaderController() const;
	VertexArrayController* GetVertexArrayController() const;
	IndexBufferController* const GetIndexBufferController() const;
	VertexBufferController* const GetVertexBufferController() const;
	VertexLayoutController* const GetVertexLayoutController() const;

	virtual void Present() = 0;
	virtual API GetApiType() const = 0;

protected:
	virtual void BindIndexBufferInternal(const IndexBuffer* indexBuffer) = 0;
	virtual void UnbindIndexBufferInternal(const IndexBuffer* indexBuffer) = 0;

	virtual void BindVertexArrayInternal(const VertexArray* vertexArray) = 0;
	virtual void UnbindVertexArrayInternal(const VertexArray* vertexArray) = 0;

	virtual void BindShaderProgramInternal(const ShaderProgram* shaderProgram) = 0;
	virtual void UnbindShaderProgramInternal(const ShaderProgram* shaderProgram) = 0;

	virtual void CleanUp();

protected:
	std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<ShaderController> m_shader_controller;
	std::unique_ptr<VertexArrayController> m_vertex_array_controller;
	std::unique_ptr<VertexBufferController> m_vertex_buffer_controller;
	std::unique_ptr<VertexLayoutController> m_vertex_layout_controller;
	std::unique_ptr<IndexBufferController> m_index_buffer_controller;
	std::unique_ptr<CommandQueue> m_command_queue;

	CommandList* m_command_list;

	const IndexBuffer* m_bound_index_buffer;
	const VertexArray* m_bound_vertex_array;
	const ShaderProgram* m_bound_shader_program;

	RasterizerSettings m_rasterizer_settings;
	bool m_is_vsync;

	Format m_back_buffer_format;
	Format m_depth_stencil_format;

	const unsigned int MAX_NR_OF_FRAMES_IN_FLIGHT = 3;

private:
	static Context* s_current;

};
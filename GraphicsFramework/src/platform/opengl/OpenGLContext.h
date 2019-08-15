#pragma once

#include "graphics/Context.h"

class Window;
class OpenGLCommandQueue;
class OpenGLCommandList;

using HandleGLRenderingContext = HGLRC;

class OpenGLContext : public Context
{
public: 
	OpenGLContext(Window* window);
	virtual ~OpenGLContext();

	virtual void Initialize() override;
	virtual void Present() override;
	API GetApiType() const;

	void InitializeRasterizer();

	OpenGLCommandList* GetCommandList() const;

protected:
	void BindIndexBufferInternal(const IndexBuffer* indexBuffer) override;
	void UnbindIndexBufferInternal(const IndexBuffer* indexBuffer) override;
	void BindVertexArrayInternal(const VertexArray* vertexArray) override;
	void UnbindVertexArrayInternal(const VertexArray* vertexArray) override;
	void BindShaderProgramInternal(const ShaderProgram* shaderProgram) override;
	void UnbindShaderProgramInternal(const ShaderProgram* shaderProgram) override;
private:
	// Handle to device context
	HDC m_hdc;
	HandleGLRenderingContext m_hglrc;
	Window* m_window;

	std::unique_ptr<OpenGLCommandQueue> m_command_queue;
	OpenGLCommandList* m_command_list;
};
#pragma once

#include "graphics/Context.h"

class Window;
class GLCommandQueue;
class GLCommandList;
class GLDirectCommandList;
class Command;

using HandleGLRenderingContext = HGLRC;

class GLContext : public Context
{
public: 
	GLContext(Window* window);
	virtual ~GLContext();

	virtual void Initialize() override;

	virtual void Begin() override;
	virtual void Present() override;
	virtual void End() override;
	
	API GetApiType() const;

	void InitializeRasterizer();

	GLCommandList* GetCommandList() const;
	std::unique_ptr<GLDirectCommandList> CreateDirectCommandList() const;
	void ExecuteDirectCommand(std::unique_ptr<Command> command) const;

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

	//std::unique_ptr<GLCommandQueue> m_command_queue;
	//GLCommandList* m_command_list;
};
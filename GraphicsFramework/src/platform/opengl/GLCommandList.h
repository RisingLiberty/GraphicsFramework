#pragma once

#include "graphics/RasterizerSettings.h"
#include "graphics/CommandList.h"

class GLCommandList : public CommandList
{
public:
	GLCommandList();
	virtual ~GLCommandList();

	unsigned int CreateBuffer();
	unsigned int CreateShader(unsigned int type);
	unsigned int CreateProgram();
	unsigned int CreateVertexArray();

	//void DeleteBuffer(unsigned int id);
	void DeleteShader(unsigned int id);
	void DeleteProgram(unsigned int id);
	void DeleteVertexArray(unsigned int id);

	void SetClearColor(std::array<float, 4> rgba);
	void SetClearColor(float r, float g, float b, float a);
	//void SetVertexBufferDataDynamic(const void* data, unsigned int size, unsigned int usage);
	//void SetIndexBufferDataDynamic(const void* data, unsigned int size, unsigned int usage);

	//No access on cpu side data access by default
	//void SetVertexBufferDataStatic(const void* data, unsigned int size, unsigned int usage = 0);
	//void SetIndexBufferDataStatic(const void* data, unsigned int size, unsigned int usage = 0);
	void SetRasterizerState(const RasterizerSettings& settings);
	void SetShaderSource(unsigned int id, const char* code);

	void CompileShader(unsigned int id);
	void AttachShader(unsigned int programId, unsigned int shaderId);
	void LinkProgram(unsigned int id);
	void ValidateProgram(unsigned int id);

	int GetShaderCompileStatus(unsigned int id);
	int GetShaderInfoLogLength(unsigned int id);
	std::string GetShaderInfoLog(unsigned int id);

	int GetProgramLinkStatus(unsigned int id);
	int GetProgramInfoLength(unsigned int id);
	std::string GetProgramInfoLog(unsigned int id);

	unsigned int GetUniformLocation(unsigned int id, const char* name);

	//void BindVertexBuffer(unsigned int id);
	//void BindVertexArray(unsigned int id);
	//void BindIndexBuffer(unsigned int id);
	void BindShaderProgram(unsigned int id);

	//void DrawElements(unsigned int topology, unsigned int count, unsigned int format);

	void* MapVertexBuffer(unsigned int size, unsigned int usage = GL_MAP_READ_BIT);
	void* MapIndexBuffer(unsigned int size, unsigned int usage = GL_MAP_READ_BIT);

	void UnmapVertexBuffer();
	void UnmapIndexBuffer();

	void ClearColorBuffer();
	void ClearDepthStencilBuffer();
	void ClearDepthBuffer();
	void ClearStencilBuffer();

private:

};
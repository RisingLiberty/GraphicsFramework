#pragma once

#include "graphics/RasterizerSettings.h"
#include "graphics/CommandList.h"

class GLCommandList : public CommandList
{
public:
	GLCommandList();
	virtual ~GLCommandList();

	unsigned int CreateShader(unsigned int type);
	unsigned int CreateProgram();
	unsigned int CreateVertexArray();

	void DeleteShader(unsigned int id);
	void DeleteProgram(unsigned int id);
	void DeleteVertexArray(unsigned int id);

	void SetClearColor(std::array<float, 4> rgba);
	void SetClearColor(float r, float g, float b, float a);

	//No access on cpu side data access by default
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

	void BindShaderProgram(unsigned int id);

	void ClearColorBuffer();
	void ClearDepthStencilBuffer();
	void ClearDepthBuffer();
	void ClearStencilBuffer();

private:

};
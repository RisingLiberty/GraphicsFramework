#include "stdafx.h"

#include "GLCommandList.h"
#include "GLHelperMethods.h"

namespace
{
	unsigned int* NULL_OFFSET = nullptr;
}

GLCommandList::GLCommandList()
{

}

GLCommandList::~GLCommandList()
{

}

unsigned int GLCommandList::CreateBuffer()
{
	unsigned int id;
	GLCALL(glGenBuffers(1, &id));
	return id;
}

unsigned int GLCommandList::CreateShader(unsigned int type)
{
	return glCreateShader(type);
}

unsigned int GLCommandList::CreateProgram()
{
	return glCreateProgram();
}

unsigned int GLCommandList::CreateVertexArray()
{
	unsigned int id;
	GLCALL(glGenVertexArrays(1, &id));
	return id;
}

void GLCommandList::DeleteBuffer(unsigned int id)
{
	GLCALL(glDeleteBuffers(1, &id));
}

void GLCommandList::DeleteShader(unsigned int id)
{
	GLCALL(glDeleteShader(id));
}

void GLCommandList::DeleteProgram(unsigned int id)
{
	GLCALL(glDeleteProgram(id));
}

void GLCommandList::DeleteVertexArray(unsigned int id)
{
	GLCALL(glDeleteVertexArrays(1, &id));
}

void GLCommandList::SetClearColor(std::array<float, 4> rgba)
{
	this->SetClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);
}

void GLCommandList::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

//void GLCommandList::SetVertexBufferDataDynamic(const void* data, unsigned int size, unsigned int usage)
//{
//	GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
//}
//
//void GLCommandList::SetIndexBufferDataDynamic(const void* data, unsigned int size, unsigned int usage)
//{
//	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage))
//}
//
//void GLCommandList::SetVertexBufferDataStatic(const void* data, unsigned int size, unsigned int usage)
//{
//	GLCALL(glBufferStorage(GL_ARRAY_BUFFER, size, data, usage));
//}
//
//void GLCommandList::SetIndexBufferDataStatic(const void* data, unsigned int size, unsigned int usage)
//{
//	GLCALL(glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, size, data, usage));
//}

void GLCommandList::SetShaderSource(unsigned int id, const char* code)
{
	GLCALL(glShaderSource(id, 1, &code, nullptr));
}

void GLCommandList::SetRasterizerState(const RasterizerSettings& settings)
{
	settings.enable_depth_clamp ? glEnable(GL_DEPTH_CLAMP) : glDisable(GL_DEPTH_CLAMP);
	settings.is_discarded ? glEnable(GL_RASTERIZER_DISCARD) : glDisable(GL_RASTERIZER_DISCARD);

	FillMode polygon_mode = settings.polygon_mode;
	CullMode cull_mode = settings.cull_mode;

	glPolygonMode(cull_mode.ToOpenGL(), polygon_mode.ToOpenGL());

	glLineWidth(settings.line_width);
	glFrontFace(settings.front_face_orientation.ToOpenGL());

	if (settings.enable_depth_bias)
	{
		switch (settings.polygon_mode)
		{
		case EFillMode::UNDEFINED:	glEnable(-1);
		case EFillMode::FILL:		glEnable(GL_POLYGON_OFFSET_FILL);
		case EFillMode::WIREFRAME:	glEnable(GL_POLYGON_OFFSET_LINE);
		case EFillMode::VERTEX:		glEnable(GL_POLYGON_OFFSET_POINT);
		}
		glPolygonOffset(settings.depth_bias_constant_factor, settings.depth_bias_slope_factor);
	}

	GLCALL(glDepthRangef(0, settings.depth_bias_clamp));

	settings.enable_multi_sample ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE);

	if (settings.enable_scissor)
		spdlog::warn("GL scissoring not implemented yet!");

}

void GLCommandList::CompileShader(unsigned int id)
{
	GLCALL(glCompileShader(id));
}

void GLCommandList::AttachShader(unsigned int programId, unsigned int shaderId)
{
	GLCALL(glAttachShader(programId, shaderId));
}

void GLCommandList::LinkProgram(unsigned int id)
{
	GLCALL(glLinkProgram(id));
}

void GLCommandList::ValidateProgram(unsigned int id)
{
	GLCALL(glValidateProgram(id));
}

int GLCommandList::GetShaderCompileStatus(unsigned int id)
{
	int result = GL_FALSE;
	GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	return result;
}

int GLCommandList::GetShaderInfoLogLength(unsigned int id)
{
	int info_length = 0;
	GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_length));
	return info_length;
}

std::string GLCommandList::GetShaderInfoLog(unsigned int id)
{
	std::string message;
	int info_length = this->GetShaderInfoLogLength(id);
	message.resize(info_length);

	GLCALL(glGetShaderInfoLog(id, info_length, nullptr, message.data()));
	return message;
}

int GLCommandList::GetProgramLinkStatus(unsigned int id)
{
	int result = 0;
	GLCALL(glGetProgramiv(id, GL_LINK_STATUS, &result));
	return result;
}

int GLCommandList::GetProgramInfoLength(unsigned int id)
{
	int message_length = 0;
	GLCALL(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &message_length));
	return message_length;
}

std::string GLCommandList::GetProgramInfoLog(unsigned int id)
{
	std::string message;
	int message_length = this->GetProgramInfoLength(id);

	message.resize(message_length);
	GLCALL(glGetProgramInfoLog(id, message_length, nullptr, message.data()));
	return message;
}

unsigned int GLCommandList::GetUniformLocation(unsigned int id, const char * name)
{
	return glGetUniformLocation(id, name);
}

void GLCommandList::BindVertexBuffer(unsigned int id)
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, id));
}

void GLCommandList::BindVertexArray(unsigned int id)
{
	GLCALL(glBindVertexArray(id));
}

void GLCommandList::BindIndexBuffer(unsigned int id)
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
}

void GLCommandList::BindShaderProgram(unsigned int id)
{
	GLCALL(glUseProgram(id));
}

//void GLCommandList::DrawElements(unsigned int topology, unsigned int count, unsigned int format)
//{
//	unsigned int* offset = NULL_OFFSET;
//	GLCALL(glDrawElements(topology, count, format, offset));
//}

void* GLCommandList::MapVertexBuffer(unsigned int size, unsigned int usage)
{
	unsigned int offset = 0;
	return glMapBufferRange(GL_ARRAY_BUFFER, offset, size, usage);
}

void* GLCommandList::MapIndexBuffer(unsigned int size, unsigned int usage)
{
	unsigned int offset = 0;
	return glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, offset, size, usage);
}

void GLCommandList::UnmapVertexBuffer()
{
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void GLCommandList::UnmapIndexBuffer()
{
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

void GLCommandList::ClearColorBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void GLCommandList::ClearDepthStencilBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GLCommandList::ClearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void GLCommandList::ClearStencilBuffer()
{
	glClear(GL_STENCIL_BUFFER_BIT);
}

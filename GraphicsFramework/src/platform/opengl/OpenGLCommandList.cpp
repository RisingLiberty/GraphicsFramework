#include "stdafx.h"

#include "OpenGLCommandList.h"
#include "OpenGLHelperMethods.h"

namespace
{
	unsigned int* NULL_OFFSET = nullptr;
}

OpenGLCommandList::OpenGLCommandList()
{

}

OpenGLCommandList::~OpenGLCommandList()
{

}

unsigned int OpenGLCommandList::CreateBuffer()
{
	unsigned int id;
	GLCALL(glGenBuffers(1, &id));
	return id;
}

unsigned int OpenGLCommandList::CreateShader(unsigned int type)
{
	return glCreateShader(type);
}

unsigned int OpenGLCommandList::CreateProgram()
{
	return glCreateProgram();
}

unsigned int OpenGLCommandList::CreateVertexArray()
{
	unsigned int id;
	GLCALL(glGenVertexArrays(1, &id));
	return id;
}

void OpenGLCommandList::DeleteBuffer(unsigned int id)
{
	GLCALL(glDeleteBuffers(1, &id));
}

void OpenGLCommandList::DeleteShader(unsigned int id)
{
	GLCALL(glDeleteShader(id));
}

void OpenGLCommandList::DeleteProgram(unsigned int id)
{
	GLCALL(glDeleteProgram(id));
}

void OpenGLCommandList::DeleteVertexArray(unsigned int id)
{
	GLCALL(glDeleteVertexArrays(1, &id));
}

void OpenGLCommandList::SetClearColor(std::array<float, 4> rgba)
{
	this->SetClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);
}

void OpenGLCommandList::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void OpenGLCommandList::SetVertexBufferDataDynamic(const void* data, unsigned int size, unsigned int usage)
{
	GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
}

void OpenGLCommandList::SetIndexBufferDataDynamic(const void* data, unsigned int size, unsigned int usage)
{
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage))
}

void OpenGLCommandList::SetVertexBufferDataStatic(const void* data, unsigned int size, unsigned int usage)
{
	GLCALL(glBufferStorage(GL_ARRAY_BUFFER, size, data, usage));
}

void OpenGLCommandList::SetIndexBufferDataStatic(const void* data, unsigned int size, unsigned int usage)
{
	GLCALL(glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, size, data, usage));
}

void OpenGLCommandList::SetShaderSource(unsigned int id, const char* code)
{
	GLCALL(glShaderSource(id, 1, &code, nullptr));
}

void OpenGLCommandList::SetRasterizerState(const RasterizerSettings& settings)
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
		spdlog::warn("OpenGL scissoring not implemented yet!");

}

void OpenGLCommandList::CompileShader(unsigned int id)
{
	GLCALL(glCompileShader(id));
}

void OpenGLCommandList::AttachShader(unsigned int programId, unsigned int shaderId)
{
	GLCALL(glAttachShader(programId, shaderId));
}

void OpenGLCommandList::LinkProgram(unsigned int id)
{
	GLCALL(glLinkProgram(id));
}

void OpenGLCommandList::ValidateProgram(unsigned int id)
{
	GLCALL(glValidateProgram(id));
}

int OpenGLCommandList::GetShaderCompileStatus(unsigned int id)
{
	int result = GL_FALSE;
	GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	return result;
}

int OpenGLCommandList::GetShaderInfoLogLength(unsigned int id)
{
	int info_length = 0;
	GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_length));
	return info_length;
}

std::string OpenGLCommandList::GetShaderInfoLog(unsigned int id)
{
	std::string message;
	int info_length = this->GetShaderInfoLogLength(id);
	message.resize(info_length);

	GLCALL(glGetShaderInfoLog(id, info_length, nullptr, message.data()));
	return message;
}

int OpenGLCommandList::GetProgramLinkStatus(unsigned int id)
{
	int result = 0;
	GLCALL(glGetProgramiv(id, GL_LINK_STATUS, &result));
	return result;
}

int OpenGLCommandList::GetProgramInfoLength(unsigned int id)
{
	int message_length = 0;
	GLCALL(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &message_length));
	return message_length;
}

std::string OpenGLCommandList::GetProgramInfoLog(unsigned int id)
{
	std::string message;
	int message_length = this->GetProgramInfoLength(id);

	message.resize(message_length);
	GLCALL(glGetProgramInfoLog(id, message_length, nullptr, message.data()));
	return message;
}

unsigned int OpenGLCommandList::GetUniformLocation(unsigned int id, const char * name)
{
	return glGetUniformLocation(id, name);
}

void OpenGLCommandList::BindVertexBuffer(unsigned int id)
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, id));
}

void OpenGLCommandList::BindVertexArray(unsigned int id)
{
	GLCALL(glBindVertexArray(id));
}

void OpenGLCommandList::BindIndexBuffer(unsigned int id)
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
}

void OpenGLCommandList::BindShaderProgram(unsigned int id)
{
	GLCALL(glUseProgram(id));
}

void OpenGLCommandList::DrawElements(unsigned int topology, unsigned int count, unsigned int format)
{
	unsigned int* offset = NULL_OFFSET;
	GLCALL(glDrawElements(topology, count, format, offset));
}

void* OpenGLCommandList::MapVertexBuffer(unsigned int size, unsigned int usage)
{
	unsigned int offset = 0;
	return glMapBufferRange(GL_ARRAY_BUFFER, offset, size, usage);
}

void* OpenGLCommandList::MapIndexBuffer(unsigned int size, unsigned int usage)
{
	unsigned int offset = 0;
	return glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, offset, size, usage);
}

void OpenGLCommandList::UnmapVertexBuffer()
{
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void OpenGLCommandList::UnmapIndexBuffer()
{
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

void OpenGLCommandList::ClearColorBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLCommandList::ClearDepthStencilBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLCommandList::ClearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void OpenGLCommandList::ClearStencilBuffer()
{
	glClear(GL_STENCIL_BUFFER_BIT);
}

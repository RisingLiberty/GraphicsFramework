#include "stdafx.h"

#include "VertexShader.h"

#include "Context.h"

#include "platform/opengl/OpenGLVertexShader.h"

VertexShader::VertexShader(const std::string& path):
	Shader(path)
{

}

VertexShader::~VertexShader()
{

}

std::unique_ptr<VertexShader> VertexShader::Create(const std::string& path)
{
	switch (Context::GetCurrent()->GetApiType())
	{
	case Context::API::OpenGL:
		return std::make_unique<OpenGLVertexShader>(path);
	}

	return nullptr;
}
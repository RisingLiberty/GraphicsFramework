#include "stdafx.h"

#include "FragmentShader.h"
#include "Context.h"

#include "platform/opengl/OpenGLFragmentShader.h"

FragmentShader::FragmentShader(const std::string& path):
	Shader(path)
{

}

FragmentShader::~FragmentShader()
{

}

std::unique_ptr<FragmentShader> FragmentShader::Create(const std::string& path)
{
	switch (Context::GetCurrent()->GetApiType())
	{
		case Context::API::OpenGL:
			return std::make_unique<OpenGLFragmentShader>(path);
	}
	return nullptr;
}
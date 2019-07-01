#include "stdafx.h"

#include <GL/glew.h>
#include <graphics/Mesh.h>

#include "HelperMethods.h"
#include "OpenGLRenderer.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLIndexBuffer.h"
#include "OpenGLVertexArray.h"

#include "OpenGLShaderProgram.h"
#include "OpenGLVertexShader.h"
#include "OpenGLFragmentShader.h"

#include <graphics/MeshFactory.h>

namespace
{
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	static ShaderProgramSource ParseShader(const std::string& filePath)
	{
		std::ifstream stream(filePath);

		enum class ShaderType
		{
			NONE = -1,

			VERTEX = 0,
			FRAGMENT = 1
		};

		if (!stream.is_open())
			return {};

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;

		while (std::getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
					type = ShaderType::VERTEX;
				else if (line.find("fragment") != std::string::npos)
					type = ShaderType::FRAGMENT;
			}
			else
			{
				ss[(int)type] << line << "\n";
			}
		}

		return { ss[0].str(), ss[1].str() };
	}

	static unsigned int CompileShader(unsigned int type, const std::string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();

		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		//error handling
		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);

			std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "framgent") << " shader" << std::endl;
			std::cout << message << std::endl;
			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		unsigned int program = glCreateProgram();

		unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(program, vs);
		glAttachShader(program, fs);

		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}
}

OpenGLRenderer::OpenGLRenderer()
{
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
}

OpenGLRenderer::~OpenGLRenderer()
{

}

void OpenGLRenderer::Present()
{
	std::unique_ptr<Mesh> mesh = MeshFactory::CreateQuad();

	OpenGLVertexArray* va = new OpenGLVertexArray();
	va->BindLayoutToBuffer(mesh->GetVertices(), mesh->GetVertexLayout());

	mesh->GetIndices()->Bind();

	std::unique_ptr<OpenGLVertexShader> vertex_shader = std::make_unique<OpenGLVertexShader>("data/shaders/opengl/vertexshader.glsl");
	std::unique_ptr<OpenGLFragmentShader> fragment_shader = std::make_unique<OpenGLFragmentShader>("data/shaders/opengl/fragmentshader.glsl");

	std::unique_ptr<OpenGLShaderProgram> program = std::make_unique<OpenGLShaderProgram>();
	program->Create({ vertex_shader->GetId(), fragment_shader->GetId() });
	program->Bind();

	program->SetVector4Uniform("u_Color", 1.0f, 0.3f, 0.8f, 1.0f);

	GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}

void OpenGLRenderer::ClearAllBuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLRenderer::ClearColorBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRenderer::ClearDepthStencilBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLRenderer::ClearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::ClearStencilBuffer()
{
	glClear(GL_STENCIL_BUFFER_BIT);
}


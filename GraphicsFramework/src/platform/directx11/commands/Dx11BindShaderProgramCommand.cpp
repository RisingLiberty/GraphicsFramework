#include "stdafx.h"

#include "Dx11BindShaderProgramCommand.h"

#include "../Dx11HelperMethods.h"

#include "../Dx11ShaderProgram.h"
#include "../Dx11VertexShader.h"
#include "../Dx11FragmentShader.h"

Dx11BindShaderProgramCommand::Dx11BindShaderProgramCommand(const ShaderProgram* shaderProgram) :
	BindShaderProgramCommand(shaderProgram)
{

}

Dx11BindShaderProgramCommand::~Dx11BindShaderProgramCommand()
{

}

void Dx11BindShaderProgramCommand::Execute()
{
	GetDx11DeviceContext()->VSSetShader(m_shader_program->GetVertexShader()->As<Dx11VertexShader>()->GetShader(), NULL, 0);
	GetDx11DeviceContext()->PSSetShader(m_shader_program->GetFragmentShader()->As<Dx11FragmentShader>()->GetShader(), NULL, 0);
}
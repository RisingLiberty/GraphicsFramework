#include "stdafx.h"

#include "Dx11ShaderProgram.h"
#include "Dx11VertexShader.h"
#include "Dx11FragmentShader.h"
#include "Dx11Context.h"
#include "Dx11ShaderUniform.h"
#include "Dx11ShaderStruct.h"
#include "Dx11ShaderCBuffer.h"
#include "Dx11ShaderTexture.h"
#include "Dx11ShaderSamplerState.h"
#include "Dx11HelperMethods.h"
#include "Dx11CommandList.h"

#include "commands/Dx11MapBufferCommand.h"
#include "commands/Dx11UnmapBufferCommand.h"
#include "commands/Dx11SetVsConstantBufferCommand.h"
#include "commands/Dx11SetPsConstantBufferCommand.h"

Dx11ShaderProgram::Dx11ShaderProgram(VertexShader* vs, FragmentShader* fs):
	ShaderProgram(vs, fs)
{
	Dx11VertexShader* dx_vs = vs->As<Dx11VertexShader>();
	Dx11FragmentShader* dx_fs = fs->As<Dx11FragmentShader>();

	Dx11ShaderParser parser;
	
	Dx11ParsedShader vs_parsed = parser.Parse(dx_vs->GetCode());
	Dx11ParsedShader fs_parsed = parser.Parse(dx_fs->GetCode());

	if (!vs_parsed.buffers.empty())
	{
		m_vs_constant_buffer = std::make_unique<Dx11ShaderCBuffer>(vs_parsed.buffers[0]);
		m_vs_constant_buffer->shader_type = Shader::Type::VERTEX;
		for (const std::unique_ptr<Dx11ShaderUniform>& uniform : m_vs_constant_buffer->uniforms)
			m_uniforms.emplace_back(std::make_unique<ShaderUniform>(uniform->name, uniform->type, uniform->size));
	}

	if (!fs_parsed.buffers.empty())
	{
		m_fs_constant_buffer = std::make_unique<Dx11ShaderCBuffer>(fs_parsed.buffers[0]);
		m_fs_constant_buffer->shader_type = Shader::Type::FRAGMENT;
		for (const std::unique_ptr<Dx11ShaderUniform>& uniform : m_fs_constant_buffer->uniforms)
			m_uniforms.emplace_back(std::make_unique<ShaderUniform>(uniform->name, uniform->type, uniform->size));
	}

	this->Bind();
}


Dx11ShaderProgram::~Dx11ShaderProgram()
{
}

void Dx11ShaderProgram::UploadVariables() const
{
	this->UploadVertexConstantBuffer();
	this->UploadFragmentConstantBuffer();
}

void Dx11ShaderProgram::UploadVertexConstantBuffer() const
{
	if (!m_vs_constant_buffer)
		return;

	byte* vertex_constant_buffer = (byte*)malloc(m_vs_constant_buffer->real_size);

	for (const std::unique_ptr<ShaderUniform>& uniform : m_uniforms)
	{
		for (std::unique_ptr<Dx11ShaderUniform>& vs_uniform : m_vs_constant_buffer->uniforms)
		{
			if (uniform->name == vs_uniform->name)
			{
				memcpy(&vertex_constant_buffer[vs_uniform->offset], uniform->data, uniform->size);
				break;
			}
		}
	}

	D3D11_MAPPED_SUBRESOURCE msr;
	ZeroMemory(&msr, sizeof(msr));

	std::unique_ptr<Dx11CommandList> direct_cmd_list = GetDx11Context()->CreateDirectCommandList();
	direct_cmd_list->Push(std::make_unique<Dx11MapBufferCommand>(m_vs_constant_buffer.get(), D3D11_MAP_WRITE_DISCARD, &msr, 0, 0));
	direct_cmd_list->Execute();
	memcpy(msr.pData, vertex_constant_buffer, m_vs_constant_buffer->size);
	GetDx11CommandList()->Push(std::make_unique<Dx11UnmapBufferCommand>(m_vs_constant_buffer.get()));

	GetDx11CommandList()->Push(std::make_unique<Dx11SetVsConstantBufferCommand>(m_vs_constant_buffer->reg, m_vs_constant_buffer->buffer.Get()));
}

void Dx11ShaderProgram::UploadFragmentConstantBuffer() const
{
	if (!m_fs_constant_buffer)
		return;

	byte* fragment_constant_buffer = (byte*)malloc(m_fs_constant_buffer->real_size);

	for (const std::unique_ptr<ShaderUniform>& uniform : m_uniforms)
	{
		for (std::unique_ptr<Dx11ShaderUniform>& fs_uniform : m_fs_constant_buffer->uniforms)
		{
			if (uniform->name == fs_uniform->name)
			{
				memcpy(&fragment_constant_buffer[fs_uniform->offset], uniform->data, uniform->size);
				break;
			}
		}
	}

	D3D11_MAPPED_SUBRESOURCE msr;
	ZeroMemory(&msr, sizeof(msr));

	std::unique_ptr<Dx11CommandList> direct_cmd_list = GetDx11Context()->CreateDirectCommandList();
	direct_cmd_list->Push(std::make_unique<Dx11MapBufferCommand>(m_fs_constant_buffer.get(), D3D11_MAP_WRITE_DISCARD, &msr, 0, 0));
	direct_cmd_list->Close();
	direct_cmd_list->Execute();

	memcpy(msr.pData, fragment_constant_buffer, m_fs_constant_buffer->size);

	direct_cmd_list = GetDx11Context()->CreateDirectCommandList();
	direct_cmd_list->Push(std::make_unique<Dx11UnmapBufferCommand>(m_fs_constant_buffer.get()));
	direct_cmd_list->Push(std::make_unique<Dx11SetPsConstantBufferCommand>(m_fs_constant_buffer->reg, m_fs_constant_buffer->buffer.Get()));
	direct_cmd_list->Close();
	direct_cmd_list->Execute();

}

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


Dx11ShaderProgram::Dx11ShaderProgram(VertexShader* vs, FragmentShader* fs):
	ShaderProgram(vs, fs)
{
	Dx11VertexShader* dx_vs = static_cast<Dx11VertexShader*>(vs);
	Dx11FragmentShader* dx_fs = static_cast<Dx11FragmentShader*>(fs);

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

void Dx11ShaderProgram::Bind() const
{
	Dx11VertexShader* dx_vs = static_cast<Dx11VertexShader*>(m_vertex_shader);
	Dx11FragmentShader* dx_fs = static_cast<Dx11FragmentShader*>(m_fragment_shader);

	GetDx11Context()->BindVertexShader(dx_vs);
	GetDx11Context()->BindFragmentShader(dx_fs);
}

void Dx11ShaderProgram::Unbind() const
{
	GetDx11Context()->BindVertexShader(nullptr);
	GetDx11Context()->BindFragmentShader(nullptr);
}

void Dx11ShaderProgram::UploadVariables()
{
	this->UploadVertexConstantBuffer();
	this->UploadFragmentConstantBuffer();
}

void Dx11ShaderProgram::UploadVertexConstantBuffer()
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

	GetDx11DeviceContext()->Map(m_vs_constant_buffer->buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msr);
	memcpy(msr.pData, vertex_constant_buffer, m_vs_constant_buffer->size);
	GetDx11DeviceContext()->Unmap(m_vs_constant_buffer->buffer.Get(), NULL);

	GetDx11DeviceContext()->VSSetConstantBuffers(m_vs_constant_buffer->reg, 1, m_vs_constant_buffer->buffer.GetAddressOf());
}

void Dx11ShaderProgram::UploadFragmentConstantBuffer()
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

	GetDx11DeviceContext()->Map(m_fs_constant_buffer->buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msr);
	memcpy(msr.pData, fragment_constant_buffer, m_fs_constant_buffer->size);
	GetDx11DeviceContext()->Unmap(m_fs_constant_buffer->buffer.Get(), NULL);

	GetDx11DeviceContext()->PSSetConstantBuffers(m_fs_constant_buffer->reg, 1, m_fs_constant_buffer->buffer.GetAddressOf());
}

Dx11VertexShader* Dx11ShaderProgram::GetDxVertexShader() const
{
	return static_cast<Dx11VertexShader*>(m_vertex_shader);
}

Dx11FragmentShader* Dx11ShaderProgram::GetDxFragmentShader() const
{
	return static_cast<Dx11FragmentShader*>(m_fragment_shader);
}
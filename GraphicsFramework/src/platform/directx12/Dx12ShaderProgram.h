#pragma once

#include "graphics/ShaderProgram.h"

class Dx12VertexShader;
class Dx12FragmentShader;

class Dx12ShaderProgram : public ShaderProgram
{
public:
	Dx12ShaderProgram(VertexShader* vs, FragmentShader* fs);
	virtual ~Dx12ShaderProgram();

	virtual void Bind() const override;
	virtual void Unbind() const override;
	virtual void UploadVariables() override;

private:
	Dx12VertexShader* GetDxVertexShader() const;
	Dx12FragmentShader* GetDxFragmentShader() const;

private:
	ComPtr<ID3D12DescriptorHeap> m_constant_buffer_heap;
	ComPtr<ID3D12RootSignature> m_root_signature;

};
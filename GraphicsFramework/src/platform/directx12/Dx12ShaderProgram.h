#pragma once

#include "graphics/ShaderProgram.h"

class Dx12VertexShader;
class Dx12FragmentShader;
class Dx12UploadBuffer;
struct Dx12ParsedShader;

class Dx12ShaderProgram : public ShaderProgram
{
public:
	Dx12ShaderProgram(VertexShader* vs, FragmentShader* fs);
	virtual ~Dx12ShaderProgram();

	virtual void UploadVariables() const override;

	Dx12UploadBuffer* GetUploadBuffer() const;
	ID3D12RootSignature* GetRootSignature() const;

private:
	void BuildRootSignature(Dx12ParsedShader* vs_parsed, Dx12ParsedShader* fs_parsed);

private:
	std::unique_ptr<Dx12UploadBuffer> m_constant_buffer;
	ComPtr<ID3D12RootSignature> m_root_signature;

};
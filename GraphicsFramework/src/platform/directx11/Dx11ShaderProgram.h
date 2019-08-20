#pragma once

#include "graphics/ShaderProgram.h"

#include "Dx11ShaderParser.h"

struct Dx11ShaderCBuffer;

class Dx11ShaderProgram : public ShaderProgram
{
public:
	Dx11ShaderProgram(VertexShader* vs, FragmentShader* fs);
	virtual ~Dx11ShaderProgram();

	virtual void UploadVariables() const override;

private:

	void UploadVertexConstantBuffer() const;
	void UploadFragmentConstantBuffer() const;

private:

	// currently only allow 1 constant buffer per shader
	std::unique_ptr<Dx11ShaderCBuffer> m_vs_constant_buffer;
	std::unique_ptr<Dx11ShaderCBuffer> m_fs_constant_buffer;
};


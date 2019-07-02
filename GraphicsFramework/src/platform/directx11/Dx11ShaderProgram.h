#pragma once

#include "graphics/ShaderProgram.h"

class Dx11ShaderProgram : public ShaderProgram
{
public:
	Dx11ShaderProgram(VertexShader* vs, FragmentShader* fs);
	virtual ~Dx11ShaderProgram();

	virtual void Bind() const override;
	virtual void Unbind() const override;

};


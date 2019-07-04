#pragma once

#include "graphics/VertexShader.h"

class Dx12VertexShader : public VertexShader
{
public:
	Dx12VertexShader(const std::string& name);
	virtual ~Dx12VertexShader();

protected:
	int Compile() override;
};
#pragma once

#include "graphics/FragmentShader.h"

class Dx12FragmentShader : public FragmentShader
{
public:
	Dx12FragmentShader(const std::string& path);
	virtual ~Dx12FragmentShader();

protected:
	int Compile() override;
};
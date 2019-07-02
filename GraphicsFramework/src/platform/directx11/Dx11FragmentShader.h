#pragma once

#include "graphics/FragmentShader.h"

class Dx11FragmentShader : public FragmentShader
{
public:
	Dx11FragmentShader(const std::string& path);
	virtual ~Dx11FragmentShader();

protected:
	int Compile() override;
};


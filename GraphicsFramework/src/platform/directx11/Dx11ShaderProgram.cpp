#include "stdafx.h"

#include "Dx11ShaderProgram.h"


Dx11ShaderProgram::Dx11ShaderProgram(VertexShader* vs, FragmentShader* fs):
	ShaderProgram(vs, fs)
{
}


Dx11ShaderProgram::~Dx11ShaderProgram()
{
}

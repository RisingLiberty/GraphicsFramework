#include "stdafx.h"

#include "Dx12VertexShader.h"

Dx12VertexShader::Dx12VertexShader(const std::string& path) :
	VertexShader(path)
{
	this->Compile();
}

Dx12VertexShader::~Dx12VertexShader()
{

}

int Dx12VertexShader::Compile()
{

}
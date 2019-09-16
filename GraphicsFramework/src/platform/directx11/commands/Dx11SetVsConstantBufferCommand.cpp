#include "stdafx.h"

#include "Dx11SetVsConstantBufferCommand.h"

#include "../Dx11HelperMethods.h"

Dx11SetVsConstantBufferCommand::Dx11SetVsConstantBufferCommand(const unsigned int reg, ID3D11Buffer* const buffer) :
	m_register(reg),
	m_buffer(buffer)
{

}

Dx11SetVsConstantBufferCommand::~Dx11SetVsConstantBufferCommand()
{

}

std::string Dx11SetVsConstantBufferCommand::ToString() const
{
	return "Set vertex shader constant buffer";
}

void Dx11SetVsConstantBufferCommand::Execute()
{
	GetDx11DeviceContext()->VSSetConstantBuffers(m_register, 1, &m_buffer);
}
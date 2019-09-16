#include "stdafx.h"

#include "Dx11SetPsConstantBufferCommand.h"

#include "../Dx11HelperMethods.h"

Dx11SetPsConstantBufferCommand::Dx11SetPsConstantBufferCommand(const unsigned int reg, ID3D11Buffer* const buffer) :
	m_register(reg),
	m_buffer(buffer)
{

}

Dx11SetPsConstantBufferCommand::~Dx11SetPsConstantBufferCommand()
{

}

std::string Dx11SetPsConstantBufferCommand::ToString() const
{
	return "Set pixel shader constant buffer";
}

void Dx11SetPsConstantBufferCommand::Execute()
{
	GetDx11DeviceContext()->PSSetConstantBuffers(m_register, 1, &m_buffer);
}
#include "stdafx.h"

#include "Dx11SetViewportCommand.h"

#include "../Dx11HelperMethods.h"

Dx11SetViewportCommand::Dx11SetViewportCommand(const D3D11_VIEWPORT& viewport) :
	m_viewport(viewport)
{

}

Dx11SetViewportCommand::~Dx11SetViewportCommand()
{

}

std::string Dx11SetViewportCommand::ToString() const
{
	return "Set viewport";
}

void Dx11SetViewportCommand::Execute()
{
	GetDx11DeviceContext()->RSSetViewports(1, &m_viewport);
}
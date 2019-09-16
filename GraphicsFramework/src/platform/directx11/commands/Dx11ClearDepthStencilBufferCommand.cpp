#include "stdafx.h"

#include "Dx11ClearDepthStencilBufferCommand.h"

#include "../Dx11HelperMethods.h"

Dx11ClearDepthStencilBufferCommand::Dx11ClearDepthStencilBufferCommand(const float depthValue, const unsigned int stencilValue, ID3D11DepthStencilView* depthStencilView) :
	ClearDepthStencilBufferCommand(depthValue, stencilValue),
	m_depth_stencil_view(depthStencilView)
{

}

Dx11ClearDepthStencilBufferCommand::~Dx11ClearDepthStencilBufferCommand()
{

}

void Dx11ClearDepthStencilBufferCommand::Execute()
{
	GetDx11DeviceContext()->ClearDepthStencilView(m_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, m_depth_value, m_stencil_value);
}
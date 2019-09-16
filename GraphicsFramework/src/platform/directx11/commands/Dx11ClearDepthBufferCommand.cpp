#include "stdafx.h"

#include "Dx11ClearDepthBufferCommand.h"

#include "../Dx11HelperMethods.h"

Dx11ClearDepthBufferCommand::Dx11ClearDepthBufferCommand(const float depthValue, ID3D11DepthView* depthView):
	ClearDepthBufferCommand(depthValue),
	m_depth_view(depthView)
{

}

Dx11ClearDepthBufferCommand::~Dx11ClearDepthBufferCommand()
{

}

void Dx11ClearDepthBufferCommand::Execute()
{
	GetDx11DeviceContext()->ClearDepthStencilView(m_depth_view, D3D11_CLEAR_DEPTH, m_depth_value, 0);
}
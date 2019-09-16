#include "stdafx.h"

#include "Dx11SetRenderTargetCommand.h"

#include "../Dx11HelperMethods.h"

Dx11SetRenderTargetCommand::Dx11SetRenderTargetCommand(ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView):
	m_render_target_view(renderTargetView),
	m_depth_stencil_view(depthStencilView)
{

}

Dx11SetRenderTargetCommand::~Dx11SetRenderTargetCommand()
{

}

std::string Dx11SetRenderTargetCommand::ToString() const
{
	return "Set render target";
}

void Dx11SetRenderTargetCommand::Execute()
{
	GetDx11DeviceContext()->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);
}
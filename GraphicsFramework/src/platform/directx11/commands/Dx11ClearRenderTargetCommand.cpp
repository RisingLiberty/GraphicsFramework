#include "stdafx.h"

#include "Dx11ClearRenderTargetCommand.h"

#include "../Dx11HelperMethods.h"

Dx11ClearRenderTargetCommand::Dx11ClearRenderTargetCommand(const std::array<float, 4>& clearColor, ID3D11RenderTargetView* renderTargetView) :
	ClearRenderTargetCommand(clearColor),
	m_render_target_view(renderTargetView)
{

}

Dx11ClearRenderTargetCommand::~Dx11ClearRenderTargetCommand()
{

}

void Dx11ClearRenderTargetCommand::Execute()
{
	GetDx11DeviceContext()->ClearRenderTargetView(m_render_target_view, m_clear_color.data());
}
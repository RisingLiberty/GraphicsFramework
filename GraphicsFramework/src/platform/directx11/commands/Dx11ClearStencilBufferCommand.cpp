#include "stdafx.h"

#include "Dx11ClearStencilBufferCommand.h"

#include "../Dx11HelperMethods.h"

Dx11ClearStencilBufferCommand::Dx11ClearStencilBufferCommand(const unsigned int stencilValue, ID3D11StencilView* stencilView):
	ClearStencilBufferCommand(stencilValue),
	m_stencil_view(stencilView)
{

}

Dx11ClearStencilBufferCommand::~Dx11ClearStencilBufferCommand()
{

}

void Dx11ClearStencilBufferCommand::Execute()
{
	GetDx11DeviceContext()->ClearDepthStencilView(m_stencil_view, D3D11_CLEAR_STENCIL, 0.0f, m_stencil_value);
}
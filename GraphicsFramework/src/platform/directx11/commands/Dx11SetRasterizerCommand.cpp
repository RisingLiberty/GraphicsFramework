#include "stdafx.h"

#include "Dx11SetRasterizerCommand.h"
#include "../Dx11HelperMethods.h"

Dx11SetRasterizerCommand::Dx11SetRasterizerCommand(const RasterizerSettings& settings) :
	SetRasterizerCommand(settings)
{

}

Dx11SetRasterizerCommand::~Dx11SetRasterizerCommand()
{

}

void Dx11SetRasterizerCommand::Execute()
{
	ComPtr<ID3D11RasterizerState> rasterizer_state;
	GetDx11Device()->CreateRasterizerState(&m_settings.ToDirectX11(), rasterizer_state.GetAddressOf());
	GetDx11DeviceContext()->RSSetState(rasterizer_state.Get());
}
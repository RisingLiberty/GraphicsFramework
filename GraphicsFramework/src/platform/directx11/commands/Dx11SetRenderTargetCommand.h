#pragma once

#include "graphics/commands/Command.h"

class Dx11SetRenderTargetCommand : public Command
{
public:
	Dx11SetRenderTargetCommand(ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView);
	virtual ~Dx11SetRenderTargetCommand();

	std::string ToString() const override;
	void Execute() override;

private:
	ID3D11RenderTargetView* m_render_target_view;
	ID3D11DepthStencilView* m_depth_stencil_view;
};
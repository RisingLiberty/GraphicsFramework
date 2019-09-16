#pragma once

#include "graphics/commands/ClearRenderTargetCommand.h"

class Dx11ClearRenderTargetCommand : public ClearRenderTargetCommand
{
public:
	Dx11ClearRenderTargetCommand(const std::array<float, 4>& clearColor, ID3D11RenderTargetView* renderTargetView);
	virtual ~Dx11ClearRenderTargetCommand();

	void Execute() override;
	
private:
	ID3D11RenderTargetView* m_render_target_view;
};
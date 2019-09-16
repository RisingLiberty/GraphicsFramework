#pragma once

#include "graphics/commands/ClearDepthStencilBufferCommand.h"

class Dx11ClearDepthStencilBufferCommand : public ClearDepthStencilBufferCommand
{
public:
	Dx11ClearDepthStencilBufferCommand(const float depthValue, const unsigned int stencilValue, ID3D11DepthStencilView* depthStencilView);
	virtual ~Dx11ClearDepthStencilBufferCommand();

	void Execute() override;

private:
	ID3D11DepthStencilView* m_depth_stencil_view;
};
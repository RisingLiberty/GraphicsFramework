#pragma once

#include "graphics/commands/ClearStencilBufferCommand.h"

typedef ID3D11DepthStencilView ID3D11StencilView;

class Dx11ClearStencilBufferCommand : public ClearStencilBufferCommand
{
public:
	Dx11ClearStencilBufferCommand(const unsigned int stencilValue, ID3D11StencilView* stencilView);
	virtual ~Dx11ClearStencilBufferCommand();

	void Execute() override;

private:
	ID3D11StencilView* m_stencil_view;
};
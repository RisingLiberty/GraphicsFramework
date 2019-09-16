#pragma once

#include "graphics/commands/ClearDepthBufferCommand.h"

typedef ID3D11DepthStencilView ID3D11DepthView;

class Dx11ClearDepthBufferCommand : public ClearDepthBufferCommand
{
public:
	Dx11ClearDepthBufferCommand(const float depthValue, ID3D11DepthView* depthView);
	virtual ~Dx11ClearDepthBufferCommand();

	void Execute() override;

private:
	ID3D11DepthView* m_depth_view;
};
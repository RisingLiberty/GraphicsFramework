#pragma once

#include "graphics/commands/Command.h"

class Dx11SetViewportCommand : public Command
{
public:
	Dx11SetViewportCommand(const D3D11_VIEWPORT& viewport);
	virtual ~Dx11SetViewportCommand();

	std::string ToString() const override;
	void Execute() override;

private:
	const D3D11_VIEWPORT m_viewport;
};
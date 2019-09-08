#pragma once

#include "Command.h"

class ClearRenderTargetCommand : public Command
{
public:
	ClearRenderTargetCommand(const std::array<float, 4>& clearColor);
	virtual ~ClearRenderTargetCommand();

	void Execute() override = 0;
	virtual std::string ToString() const override;

protected:
	std::array<float, 4> m_clear_color;
};
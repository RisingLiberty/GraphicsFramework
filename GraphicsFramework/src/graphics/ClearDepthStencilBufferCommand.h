#pragma once

#include "Command.h"

class ClearDepthStencilBufferCommand : public Command
{
public:
	ClearDepthStencilBufferCommand(float depthValue, unsigned int stencilValue);
	virtual ~ClearDepthStencilBufferCommand();

	virtual void Execute() = 0;
	virtual std::string ToString() const override;

protected:
	float m_depth_value;
	unsigned int m_stencil_value;
};
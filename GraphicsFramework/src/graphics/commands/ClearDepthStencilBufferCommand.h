#pragma once

#include "Command.h"

class ClearDepthStencilBufferCommand : public Command
{
public:
	ClearDepthStencilBufferCommand(const float depthValue, const unsigned int stencilValue);
	virtual ~ClearDepthStencilBufferCommand();

	virtual void Execute() = 0;
	virtual std::string ToString() const override;

protected:
	const float m_depth_value;
	const unsigned int m_stencil_value;
};
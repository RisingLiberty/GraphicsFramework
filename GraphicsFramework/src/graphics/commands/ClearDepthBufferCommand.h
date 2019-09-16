#pragma once

#include "Command.h"

class ClearDepthBufferCommand : public Command
{
public:
	ClearDepthBufferCommand(const float depthValue);
	virtual ~ClearDepthBufferCommand();

	virtual void Execute() override = 0;
	virtual std::string ToString() const override;

protected:
	const float m_depth_value;
};
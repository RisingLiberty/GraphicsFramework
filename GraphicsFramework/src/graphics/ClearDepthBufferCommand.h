#pragma once

#include "Command.h"

class ClearDepthBufferCommand : public Command
{
public:
	ClearDepthBufferCommand(float depthValue);
	virtual ~ClearDepthBufferCommand();

	virtual void Execute() override = 0;
	virtual std::string ToString() const override;

protected:
	float m_depth_value;
};
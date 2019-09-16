#pragma once

#include "Command.h"

#include "../BufferHelpers.h"

class DrawIndexedCommand : public Command
{
public:
	DrawIndexedCommand(const unsigned int count);
	virtual ~DrawIndexedCommand();

	void Execute() override = 0;
	virtual std::string ToString() const override;

protected:
	const unsigned int m_count;
};
#pragma once

#include "Command.h"

class ApiBufferWrapper;

class MapBufferCommand : public Command
{
public:
	MapBufferCommand(const ApiBufferWrapper* buffer, void** data);
	virtual ~MapBufferCommand();

	virtual void Execute() override = 0;
	virtual std::string ToString() const override;

protected:
	const ApiBufferWrapper* m_buffer;
	void** m_data;
};
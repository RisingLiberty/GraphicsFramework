#pragma once

#include "Command.h"

class CopyToPointerCommand : public Command
{
public:
	CopyToPointerCommand(void* dst, void** src, unsigned int size);
	virtual ~CopyToPointerCommand();

	virtual void Execute() override;
	virtual std::string ToString() const override;

private:
	void* m_destination;
	void** m_source;
	unsigned int m_size;
};
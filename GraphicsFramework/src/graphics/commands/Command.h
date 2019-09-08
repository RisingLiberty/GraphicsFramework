#pragma once

#include "../GraphicsResource.h"

class Command : public GraphicsResource<Command>
{
public:
	Command();
	virtual ~Command();

	virtual void Execute() = 0;

	virtual std::string ToString() const = 0;
};
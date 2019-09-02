#pragma once

#include "graphics/UnmapBufferCommand.h"

class GLUnmapBufferCommand : public UnmapBufferCommand
{
public:
	GLUnmapBufferCommand(const ApiBufferWrapper* buffer);
	virtual ~GLUnmapBufferCommand();

	virtual void Execute() override;
};
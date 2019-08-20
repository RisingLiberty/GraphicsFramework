#pragma once

#include "graphics/CommandQueue.h"

class OpenGLCommandList;

class OpenGLCommandQueue : public CommandQueue
{
public:
	OpenGLCommandQueue(unsigned int maxNrOfFramesInFlight);
	virtual ~OpenGLCommandQueue();

	OpenGLCommandList* GetCommandList() const;

};
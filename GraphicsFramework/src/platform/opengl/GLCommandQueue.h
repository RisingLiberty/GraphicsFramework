#pragma once

#include "graphics/CommandQueue.h"

class GLCommandList;

class GLCommandQueue : public CommandQueue
{
public:
	GLCommandQueue(unsigned int maxNrOfFramesInFlight);
	virtual ~GLCommandQueue();

	GLCommandList* GetCommandList() const;

};
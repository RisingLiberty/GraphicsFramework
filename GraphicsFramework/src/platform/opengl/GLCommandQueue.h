#pragma once

#include "graphics/CommandQueue.h"

class GLCommandList;
class GLDirectCommandList;

class GLCommandQueue : public CommandQueue
{
public:
	GLCommandQueue(unsigned int maxNrOfFramesInFlight);
	virtual ~GLCommandQueue();

	GLCommandList* GetCommandList() const;
	std::unique_ptr<GLDirectCommandList> CreateDirectCommandList() const;

};
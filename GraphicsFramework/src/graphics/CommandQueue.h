#pragma once

#include "GraphicsResource.h"

class CommandList;

class CommandQueue : public GraphicsResource<CommandQueue>
{
public:
	CommandQueue(unsigned int maxNrOfFramesInFlight);
	virtual ~CommandQueue();

	void Push(std::unique_ptr<CommandList> commandList);

protected:
	std::vector<std::unique_ptr<CommandList>> m_command_lists;
	unsigned int m_max_nr_of_frames_in_flight;
};
#include "stdafx.h"

#include "CommandQueue.h"

#include "CommandList.h"

CommandQueue::CommandQueue(unsigned int maxNrOfFramesInFlight):
	m_max_nr_of_frames_in_flight(maxNrOfFramesInFlight)
{
	m_command_lists.resize(m_max_nr_of_frames_in_flight);
}

CommandQueue::~CommandQueue()
{

}

void CommandQueue::Push(std::unique_ptr<CommandList> commandList)
{
	m_command_lists.push_back(std::move(commandList));
}
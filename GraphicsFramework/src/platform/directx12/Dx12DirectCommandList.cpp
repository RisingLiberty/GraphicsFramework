#include "stdafx.h"

#include "Dx12DirectCommandList.h"
#include "Dx12CommandQueue.h"

Dx12DirectCommandList::Dx12DirectCommandList(const Dx12CommandQueue* queue):
	m_queue(queue)
{
	m_has_executed = false;
}

Dx12DirectCommandList::~Dx12DirectCommandList()
{
	if (!m_has_executed)
	{
		this->Close();
		this->Execute();
	}
}

void Dx12DirectCommandList::Execute()
{
    CommandList::Execute();
	m_queue->Execute(this);
	m_queue->Flush();
	m_has_executed = true;
}

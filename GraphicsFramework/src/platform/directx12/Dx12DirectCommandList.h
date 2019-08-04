#pragma once

#include "Dx12CommandList.h"

class Dx12CommandQueue;

class Dx12DirectCommandList : public Dx12CommandList
{
public:
	Dx12DirectCommandList(const Dx12CommandQueue* queue);
	virtual ~Dx12DirectCommandList();

	void Execute() override;

private:
	const Dx12CommandQueue* m_queue;
	bool m_has_executed;
};
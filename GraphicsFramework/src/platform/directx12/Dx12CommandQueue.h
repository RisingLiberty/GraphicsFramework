#pragma once

#include "graphics/CommandQueue.h"

class Dx12CommandList;

class Dx12CommandQueue : public CommandQueue
{
public:
	Dx12CommandQueue(unsigned int maxNrOfFramesInFlight);
	virtual ~Dx12CommandQueue();

	void Flush() const;
	void Execute(Dx12CommandList* cmdList) const;
	void Execute();
	unsigned int Signal() const;

	std::unique_ptr<Dx12CommandList> CreateDirectCommandList() const;
	Dx12CommandList* GetApiCommandList() const;
	ID3D12CommandQueue* GetApiQueue() const;

private:
	ComPtr<ID3D12CommandQueue> m_queue;
	ComPtr<ID3D12Fence> m_fence;
	mutable unsigned int m_fence_value;
};
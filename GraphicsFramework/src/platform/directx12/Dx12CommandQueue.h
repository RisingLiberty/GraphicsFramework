#pragma once

class Dx12CommandList;

class Dx12CommandQueue
{
public:
	Dx12CommandQueue();
	~Dx12CommandQueue();

	void Flush();
	void Push(std::unique_ptr<Dx12CommandList> commandList);
	void Execute();
	unsigned int Signal();

	Dx12CommandList* GetApiCommandList() const;
	ID3D12CommandQueue* GetApiQueue() const;

private:
	std::vector<std::unique_ptr<Dx12CommandList>> m_command_lists;
	ComPtr<ID3D12CommandQueue> m_queue;
	ComPtr<ID3D12Fence> m_fence;
	unsigned int m_fence_value;
};
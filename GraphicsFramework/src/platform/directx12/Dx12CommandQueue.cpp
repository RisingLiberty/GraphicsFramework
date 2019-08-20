#include "stdafx.h"

#include "Dx12CommandQueue.h"
#include "Dx12HelperMethods.h"
#include "Dx12CommandList.h"
#include "Dx12DirectCommandList.h"

Dx12CommandQueue::Dx12CommandQueue(unsigned int maxNrOfFramesInFlight):
	CommandQueue(maxNrOfFramesInFlight)
{
	m_fence_value = 0;

	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	DXCALL(GetDx12Device()->CreateCommandQueue(&desc, IID_PPV_ARGS(m_queue.ReleaseAndGetAddressOf())));

	DXCALL(GetDx12Device()->CreateFence(0, D3D12_FENCE_FLAG_NONE,
	IID_PPV_ARGS(&m_fence)));

	m_command_lists.resize(1);
	m_command_lists[0] = std::make_unique<Dx12CommandList>();
}

Dx12CommandQueue::~Dx12CommandQueue()
{

}

void Dx12CommandQueue::Flush() const
{
	unsigned int fence_value = this->Signal();
	
	if (m_fence->GetCompletedValue() < fence_value)
	{
		HANDLE event_handle = CreateEventExW(nullptr, false, false, EVENT_ALL_ACCESS);
		DXCALL(m_fence->SetEventOnCompletion(fence_value, event_handle));

		// Wait till the GPU hits current fence event.
		WaitForSingleObject(event_handle, INFINITE);
		CloseHandle(event_handle);
	}
}

unsigned int Dx12CommandQueue::Signal() const
{
	DXCALL(m_queue->Signal(m_fence.Get(), ++m_fence_value));
	return m_fence_value;
}

std::unique_ptr<Dx12CommandList> Dx12CommandQueue::CreateDirectCommandList() const
{
	return std::make_unique<Dx12DirectCommandList>(this);
}

void Dx12CommandQueue::Execute(Dx12CommandList* cmdList) const
{
	ID3D12CommandList* cmd_list[] = { cmdList->GetApiCommandList() };
	m_queue->ExecuteCommandLists(1, cmd_list);
}

void Dx12CommandQueue::Execute()
{
	std::vector<ID3D12CommandList*> cmds_lists(m_command_lists.size());
	for (unsigned int i = 0; i < m_command_lists.size(); ++i)
	{
		Dx12CommandList* command_list = m_command_lists[i]->As<Dx12CommandList>();
		if (!command_list->IsClosed())
			command_list->Close();
		command_list->Execute();
		
		cmds_lists[i] = (command_list->GetApiCommandList());
	}
	m_queue->ExecuteCommandLists((unsigned int)cmds_lists.size(), cmds_lists.data());
}

Dx12CommandList* Dx12CommandQueue::GetApiCommandList() const
{
	return m_command_lists.front()->As<Dx12CommandList>();
}

ID3D12CommandQueue* Dx12CommandQueue::GetApiQueue() const
{
	return m_queue.Get();
}
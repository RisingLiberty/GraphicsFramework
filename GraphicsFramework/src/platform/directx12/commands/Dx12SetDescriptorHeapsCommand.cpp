#include "stdafx.h"

#include "Dx12SetDescriptorHeapsCommand.h"

#include "../Dx12HelperMethods.h"
#include "../Dx12DescriptorHeap.h"

Dx12SetDescriptorHeapsCommand::Dx12SetDescriptorHeapsCommand(const std::vector<Dx12DescriptorHeap*>& descriptorHeap):
    m_descriptor_heaps(descriptorHeap)
{

}

Dx12SetDescriptorHeapsCommand::~Dx12SetDescriptorHeapsCommand()
{

}

void Dx12SetDescriptorHeapsCommand::Execute()
{
    std::vector<ID3D12DescriptorHeap*> desc_heaps(m_descriptor_heaps.size());

    for (unsigned int i = 0; i < desc_heaps.size(); ++i)
        desc_heaps[i] = m_descriptor_heaps[i]->GetHeap();

    GetDx12CommandList()->GetApiCommandList()->SetDescriptorHeaps((UINT)m_descriptor_heaps.size(), desc_heaps.data());
}
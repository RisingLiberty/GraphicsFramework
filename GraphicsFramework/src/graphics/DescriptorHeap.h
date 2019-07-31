#pragma once

enum class DescriptorHeapType
{
	RENDER_TARGET,
	DEPTH_STENCIL,
	CBV_SRV_UAV
};


class DescriptorHeap
{
public:
	DescriptorHeap(unsigned int count, DescriptorHeapType type);
	virtual ~DescriptorHeap();

	unsigned int GetCount() const;

private:
	unsigned int m_count;
	DescriptorHeapType m_type;
};
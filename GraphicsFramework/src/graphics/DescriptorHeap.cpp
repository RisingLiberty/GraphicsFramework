#include "stdafx.h"

#include "DescriptorHeap.h"

DescriptorHeap::DescriptorHeap(unsigned int count, DescriptorHeapType type) :
	m_count(count),
	m_type(type)
{

}

DescriptorHeap::~DescriptorHeap()
{

}

unsigned int DescriptorHeap::GetCount() const
{
	return m_count;
}
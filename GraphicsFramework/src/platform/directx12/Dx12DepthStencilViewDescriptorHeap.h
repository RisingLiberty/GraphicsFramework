#pragma once

#include "Dx12DescriptorHeap.h"

class Dx12DepthStencilViewDescriptorHeap : public Dx12DescriptorHeap
{
public:
	Dx12DepthStencilViewDescriptorHeap(unsigned int count);
	virtual ~Dx12DepthStencilViewDescriptorHeap();
};
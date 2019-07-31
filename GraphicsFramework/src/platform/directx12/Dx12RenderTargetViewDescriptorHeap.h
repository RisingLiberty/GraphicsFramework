#pragma once

#include "Dx12DescriptorHeap.h"

class Dx12RenderTargetViewDescriptorHeap : public Dx12DescriptorHeap
{
public:
	Dx12RenderTargetViewDescriptorHeap(unsigned int count);
	virtual ~Dx12RenderTargetViewDescriptorHeap();

private:

};
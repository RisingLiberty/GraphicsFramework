#pragma once

#include "Dx12DescriptorHeap.h"

class Dx12CbvSrvUavDescriptorHeap : public Dx12DescriptorHeap
{
public:
	Dx12CbvSrvUavDescriptorHeap(unsigned int count, bool isVisibleByShader);
	virtual ~Dx12CbvSrvUavDescriptorHeap();

};
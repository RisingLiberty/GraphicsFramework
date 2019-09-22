#pragma once

#include "Dx12CommandQueue.h"
#include "Dx12CommandList.h"

#include "Dx12Context.h"

bool CheckD3D12Error(HRESULT result);
Dx12Context* GetDx12Context();
ID3D12Device* GetDx12Device();
Dx12CommandQueue* GetDx12CommandQueue();
Dx12CommandList* GetDx12CommandList();
IDXGIFactory* GetDxgiFactory();

#ifndef DXCALL
#define DXCALL(function)\
{\
	HRESULT __hr = function;\
	ASSERT(CheckD3D12Error(__hr), "DirectX Error!"); \
}
#endif

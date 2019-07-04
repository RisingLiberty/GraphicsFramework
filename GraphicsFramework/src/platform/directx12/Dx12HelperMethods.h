#pragma once

bool CheckD3D12Error(HRESULT result);
class Dx12Context* GetDx12Context();
ID3D12Device* GetDx12Device();
ID3D12GraphicsCommandList* GetDx12CommandList();

#ifndef DXCALL
#define DXCALL(function)\
{\
	HRESULT __hr = function;\
	ASSERT(CheckD3D12Error(__hr), "DirectX Error!"); \
}
#endif

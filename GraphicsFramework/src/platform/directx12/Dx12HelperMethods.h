#pragma once

bool CheckD3D12Error(HRESULT result);
class Dx12Context* GetDx12Context();
ID3D12Device* GetDx12Device();
ID3D12GraphicsCommandList* GetDx12CommandList();
ComPtr<ID3D12Resource> CreateDefaultBuffer(const void* data, unsigned int size, ComPtr<ID3D12Resource>& uploadBuffer);

#ifndef DXCALL
#define DXCALL(function)\
{\
	HRESULT __hr = function;\
	ASSERT(CheckD3D12Error(__hr), "DirectX Error!"); \
}
#endif

#include "stdafx.h"

#include "Dx12HelperMethods.h"

#include "Dx12Context.h"
#include "Dx12Context.h"

#ifndef D3D12_ERROR_FILE_NOT_FOUND
#define D3D12_ERROR_FILE_NOT_FOUND D3D11_ERROR_FILE_NOT_FOUND
#endif

#ifndef D3D12_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS
#define D3D12_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS
#endif

#ifndef D3D12_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS
#define D3D12_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS
#endif

bool CheckD3D12Error(HRESULT result)
{
	if (result == S_OK)
		return true;

	switch (result)
	{
	case D3D12_ERROR_FILE_NOT_FOUND:
		break;
	case D3D12_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
		break;
	case D3D12_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
		break;
	case DXGI_ERROR_INVALID_CALL:
		break;
	case DXGI_ERROR_WAS_STILL_DRAWING:
		break;
	case E_FAIL:
		break;
	case E_INVALIDARG:
		break;
	case E_OUTOFMEMORY:
		break;
	case E_NOTIMPL:
		break;
	case S_FALSE:
		break;
	}

	return false;
}

Dx12Context* GetDx12Context()
{
	return static_cast<Dx12Context*>(Context::GetCurrent());
}

ID3D12Device* GetDx12Device()
{
	return GetDx12Context()->GetDevice();
}

ID3D12CommandQueue* GetDx12CommandQueue()
{
	return GetDx12Context()->GetCommandQueue();
}

ID3D12GraphicsCommandList* GetDx12CommandList()
{
	return GetDx12Context()->GetCommandList();
}

IDXGIFactory* GetDxgiFactory()
{
	return GetDx12Context()->GetDxgiFactory();
}

ComPtr<ID3D12Resource> CreateDefaultBuffer(const void* data, unsigned int size, ComPtr<ID3D12Resource>& uploadBuffer)
{
	ID3D12Device* device = GetDx12Device();
	ID3D12GraphicsCommandList* command_list = GetDx12CommandList();

	ComPtr<ID3D12Resource> defaultBuffer;

	// Create the actual default buffer resource.
	DXCALL(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(size),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(defaultBuffer.GetAddressOf())));

	// In order to copy CPU memory data into our default buffer, we need to create
	// an intermediate upload heap. 
	DXCALL(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(size),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(uploadBuffer.GetAddressOf())));


	// Describe the data we want to copy into the default buffer.
	D3D12_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pData = data;
	subResourceData.RowPitch = size;
	subResourceData.SlicePitch = subResourceData.RowPitch;

	// Schedule to copy the data to the default buffer resource.  At a high level, the helper function UpdateSubresources
	// will copy the CPU memory into the intermediate upload heap.  Then, using ID3D12CommandList::CopySubresourceRegion,
	// the intermediate upload heap data will be copied to mBuffer.
	command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
	UpdateSubresources<1>(command_list, defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);
	command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));

	// Note: uploadBuffer has to be kept alive after the above function calls because
	// the command list has not been executed yet that performs the actual copy.
	// The caller can Release the uploadBuffer after it knows the copy has been executed.


	return defaultBuffer;
}
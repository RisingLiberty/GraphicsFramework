#include "stdafx.h"

#include "Dx12HelperMethods.h"

#include "Dx12Context.h"
#include "Dx12CommandList.h"
#include "Dx12Resource.h"

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

Dx12CommandQueue* GetDx12CommandQueue()
{
	return GetDx12Context()->GetCommandQueue();
}

Dx12CommandList* GetDx12CommandList()
{
	return GetDx12Context()->GetCommandList();
}

IDXGIFactory* GetDxgiFactory()
{
	return GetDx12Context()->GetDxgiFactory();
}

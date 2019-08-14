#include "stdafx.h"

#include "Dx11HelperMethods.h"

#include "Dx11Context.h"

bool CheckD3D11Error(HRESULT result)
{
	if (result == S_OK)
		return true;

	switch (result)
	{
	case D3D11_ERROR_FILE_NOT_FOUND:
		spdlog::error("File not found!");
		break;
	case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
		spdlog::error("Too many unique state objects!");
		break;
	case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
		spdlog::error("Too many unique view objects!");
		break;
	case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
		spdlog::error("Deferred context map without initial discard!");
		break;
	case DXGI_ERROR_INVALID_CALL:
		spdlog::error("invalid call!");
		break;
	case DXGI_ERROR_WAS_STILL_DRAWING:
		spdlog::error("was still drawing!");
		break;
	case E_FAIL:
		spdlog::error("Fail!");
		break;
	case E_INVALIDARG:
		spdlog::error("Invalid Argument!");
		break;
	case E_OUTOFMEMORY:
		spdlog::error("Out of memory!");
		break;
	case E_NOTIMPL:
		spdlog::error("Not implemented!");
		break;
	case S_FALSE:
		spdlog::error("False!");
		break;
	}

	return false;
}

Dx11Context* GetDx11Context()
{
	return static_cast<Dx11Context*>(Context::GetCurrent());
}

ID3D11Device* GetDx11Device()
{
	return GetDx11Context()->GetDevice();
}

class Dx11CommandList* GetDx11CommandList()
{
	return GetDx11Context()->GetCommandList();
}

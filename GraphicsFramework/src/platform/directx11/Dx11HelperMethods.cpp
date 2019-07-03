#include "stdafx.h"

#include "Dx11HelperMethods.h"

#include "Dx11Context.h"

bool CheckD3DError(HRESULT result)
{
	if (result == S_OK)
		return true;

	switch (result)
	{
	case D3D11_ERROR_FILE_NOT_FOUND:
		break;
	case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
		break;
	case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
		break;
	case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
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

Dx11Context* GetDx11Context()
{
	return dynamic_cast<Dx11Context*>(Context::GetCurrent());
}

ID3D11Device* GetDx11Device()
{
	return GetDx11Context()->GetDevice();
}

ID3D11DeviceContext* GetDx11DeviceContext()
{
	return GetDx11Context()->GetDeviceContext();
}
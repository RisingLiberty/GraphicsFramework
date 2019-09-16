#pragma once

#include "Dx11Context.h"

bool CheckD3D11Error(HRESULT result);
Dx11Context* GetDx11Context();
ID3D11Device* GetDx11Device();
ID3D11DeviceContext* GetDx11DeviceContext();
class Dx11CommandList* GetDx11CommandList();

#ifndef DXCALL
#define DXCALL(function)\
{\
	HRESULT __hr = function;\
	ASSERT(CheckD3D11Error(__hr), "DirectX Error!"); \
}
#endif

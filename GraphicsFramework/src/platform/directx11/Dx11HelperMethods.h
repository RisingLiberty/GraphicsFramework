#pragma once

bool CheckD3D11Error(HRESULT result);
class Dx11Context* GetDx11Context();
ID3D11Device* GetDx11Device();
ID3D11DeviceContext* GetDx11DeviceContext();

#ifndef DXCALL
#define DXCALL(function)\
{\
	HRESULT __hr = function;\
	ASSERT(CheckD3D11Error(__hr), "DirectX Error!"); \
}
#endif

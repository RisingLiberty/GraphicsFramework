#pragma once

bool CheckD3DError(HRESULT result);
class Dx11Context* GetDx11Context();
ID3D11Device* GetDx11Device();
ID3D11DeviceContext* GetDx11DeviceContext();


#define DXCALL(function)\
{\
	HRESULT __hr = function;\
	ASSERT(CheckD3DError(__hr), "DirectX Error!"); \
}


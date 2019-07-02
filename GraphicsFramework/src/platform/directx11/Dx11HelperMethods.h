#pragma once

bool CheckD3DError(HRESULT result);
class Dx11Context* GetDx11Context();

#define DXCALL(function)\
{\
	HRESULT __hr = function;\
	ASSERT(CheckD3DError(__hr), "DirectX Error!"); \
}


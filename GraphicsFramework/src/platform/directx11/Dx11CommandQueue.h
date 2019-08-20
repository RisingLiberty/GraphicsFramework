#pragma once

#include "graphics/CommandQueue.h"

class Dx11CommandList;

class Dx11CommandQueue : public CommandQueue
{
public:
	Dx11CommandQueue(unsigned int maxNrOfFramesInFlight);
	virtual ~Dx11CommandQueue();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

	Dx11CommandList* GetCommandList() const;

private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_device_context;
	ComPtr<ID3D11Debug> m_debug_layer;
	D3D_FEATURE_LEVEL m_feature_level;
};
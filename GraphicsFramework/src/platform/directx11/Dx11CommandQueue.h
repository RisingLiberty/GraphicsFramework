#pragma once

class Dx11CommandList;

class Dx11CommandQueue
{
public:
	Dx11CommandQueue();
	~Dx11CommandQueue();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

	Dx11CommandList* GetCommandList() const;

private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_device_context;
	ComPtr<ID3D11Debug> m_debug_layer;
	D3D_FEATURE_LEVEL m_feature_level;

	std::vector<std::unique_ptr<Dx11CommandList>> m_command_lists;
};
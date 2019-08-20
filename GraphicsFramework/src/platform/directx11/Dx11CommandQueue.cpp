#include "stdafx.h"

#include "Dx11CommandQueue.h"
#include "Dx11HelperMethods.h"

#include "Dx11CommandList.h"

Dx11CommandQueue::Dx11CommandQueue(unsigned int maxNrOfFramesInFlight):
	CommandQueue(maxNrOfFramesInFlight)
{
	UINT creationFlags = 0; //D3D11_CREATE_DEVICE_BGRA_SUPPORT <-- not needed, but good to have on stand by
#if defined(_DEBUG)
	// Let's DirectX log to visual studio's output tab.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	DXCALL(D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		creationFlags,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		m_device.ReleaseAndGetAddressOf(),
		&m_feature_level,
		m_device_context.ReleaseAndGetAddressOf()
	));

#if defined _DEBUG
	DXCALL(m_device->QueryInterface(IID_PPV_ARGS(m_debug_layer.ReleaseAndGetAddressOf())));
	DXCALL(m_debug_layer->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL));

	//TODO: Look up further information about this
	ID3D11InfoQueue* info_queue;
	DXCALL(m_device->QueryInterface(IID_PPV_ARGS(&info_queue)));
	D3D11_MESSAGE_ID hide[] = { D3D11_MESSAGE_ID_DEVICE_DRAW_SAMPLER_NOT_SET };
	D3D11_INFO_QUEUE_FILTER filter;
	ZeroMemory(&filter, sizeof(filter));
	filter.DenyList.NumIDs = 1;
	filter.DenyList.pIDList = hide;
	DXCALL(info_queue->AddStorageFilterEntries(&filter));
#endif

	m_command_lists.resize(1);
	m_command_lists[0] = std::make_unique<Dx11CommandList>(m_device_context);
}

Dx11CommandQueue::~Dx11CommandQueue()
{

}

ID3D11Device* Dx11CommandQueue::GetDevice() const
{
	return m_device.Get();
}

ID3D11DeviceContext* Dx11CommandQueue::GetDeviceContext() const
{
	return m_device_context.Get();
}

Dx11CommandList* Dx11CommandQueue::GetCommandList() const
{
	return m_command_lists.front()->As<Dx11CommandList>();
}
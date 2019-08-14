#include "stdafx.h"

#include "Dx11CommandList.h"
#include "Dx11HelperMethods.h"

Dx11CommandList::Dx11CommandList(ComPtr<ID3D11DeviceContext> deviceContext):
	m_device_context(deviceContext)
{
}

Dx11CommandList::~Dx11CommandList()
{
}

ID3D11DeviceContext* Dx11CommandList::GetDeviceContext() const
{
	return m_device_context.Get();
}

void Dx11CommandList::SetViewport(D3D11_VIEWPORT& viewport)
{
	m_device_context->RSSetViewports(1, &viewport);
}

void Dx11CommandList::SetRasterizerState(ID3D11RasterizerState* rasterizerState)
{
	m_device_context->RSSetState(rasterizerState);
}

void Dx11CommandList::SetVertexShader(ID3D11VertexShader* vs)
{
	m_device_context->VSSetShader(vs, NULL, 0);
}

void Dx11CommandList::SetVSConstantBuffer(unsigned int reg, unsigned int numBuffers, ID3D11Buffer* buffer)
{
	m_device_context->VSSetConstantBuffers(reg, numBuffers, &buffer);
}

void Dx11CommandList::SetPixelShader(ID3D11PixelShader* ps)
{
	m_device_context->PSSetShader(ps, NULL, 0);
}

void Dx11CommandList::SetPSConstantBuffer(unsigned int reg, unsigned int numBuffers, ID3D11Buffer* buffer)
{
	m_device_context->PSSetConstantBuffers(reg, numBuffers, &buffer);
}

void Dx11CommandList::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	m_device_context->IASetPrimitiveTopology(topology);
}

void Dx11CommandList::SetIndexBuffer(ID3D11Buffer* buffer, DXGI_FORMAT format, UINT offset)
{
	m_device_context->IASetIndexBuffer(buffer, format, offset);
}

void Dx11CommandList::SetVertexBuffer(ID3D11Buffer* buffer, unsigned int stride)
{
	unsigned int offset = 0;
	unsigned int input_slot = 0;
	m_device_context->IASetVertexBuffers(input_slot, 1, &buffer, &stride, &offset);
}

void Dx11CommandList::SetInputLayout(ID3D11InputLayout* inputLayout)
{
	m_device_context->IASetInputLayout(inputLayout);
}

void Dx11CommandList::SetRenderTarget(ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView)
{
	m_device_context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
}

void Dx11CommandList::ClearRenderTargetView(ID3D11RenderTargetView* renderTargetView, std::array<float, 4> clearColor)
{
	m_device_context->ClearRenderTargetView(renderTargetView, &clearColor[0]);
}

void Dx11CommandList::ClearDepthStencilView(ID3D11DepthStencilView* depthStencilView)
{
	m_device_context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Dx11CommandList::ClearDepthView(ID3D11DepthStencilView* depthStencilView)
{
	m_device_context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Dx11CommandList::ClearStencilView(ID3D11DepthStencilView* depthStencilView)
{
	m_device_context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Dx11CommandList::DrawIndexed(unsigned int indexCount, unsigned int startIndexLocation, unsigned int baseVertexLocation)
{
	m_device_context->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

void Dx11CommandList::Map(ID3D11Buffer* buffer, D3D11_MAP mapType, D3D11_MAPPED_SUBRESOURCE& mappedSubResource, unsigned int subResource, unsigned int mapFlags)
{
	DXCALL(m_device_context->Map(buffer, subResource, mapType, mapFlags, &mappedSubResource));
}

void Dx11CommandList::Unmap(ID3D11Buffer* buffer, unsigned int subResource)
{
	m_device_context->Unmap(buffer, subResource);
}

void Dx11CommandList::CopyResource(ID3D11Resource* dest, ID3D11Resource* src)
{
	m_device_context->CopyResource(dest, src);
}

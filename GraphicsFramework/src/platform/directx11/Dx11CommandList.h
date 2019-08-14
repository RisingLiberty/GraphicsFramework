#pragma once

class Dx11CommandList
{
public:
	Dx11CommandList(ComPtr<ID3D11DeviceContext> deviceContext);
	~Dx11CommandList();

	ID3D11DeviceContext* GetDeviceContext() const;

	void SetViewport(D3D11_VIEWPORT& viewport);
	void SetRasterizerState(ID3D11RasterizerState* rasterizerState);
	void SetVertexShader(ID3D11VertexShader* vs);
	void SetVSConstantBuffer(unsigned int reg, unsigned int numBuffers, ID3D11Buffer* buffer);
	void SetPixelShader(ID3D11PixelShader* ps);
	void SetPSConstantBuffer(unsigned int reg, unsigned int numBuffers, ID3D11Buffer* buffer);
	void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology);
	void SetIndexBuffer(ID3D11Buffer* buffer, DXGI_FORMAT format, UINT offset);
	void SetVertexBuffer(ID3D11Buffer* buffer, unsigned int stride);
	void SetInputLayout(ID3D11InputLayout* inputLayout);
	void SetRenderTarget(ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView);
	
	void ClearRenderTargetView(ID3D11RenderTargetView* renderTargetView, std::array<float, 4> clearColor);
	void ClearDepthStencilView(ID3D11DepthStencilView* depthStencilView);
	void ClearDepthView(ID3D11DepthStencilView* depthStencilView);
	void ClearStencilView(ID3D11DepthStencilView* depthStencilView);
	
	void DrawIndexed(unsigned int indexCount, unsigned int startIndexLocation = 0, unsigned int baseVertexLocation = 0);

	void Map(ID3D11Buffer* buffer, D3D11_MAP mapType, D3D11_MAPPED_SUBRESOURCE& mappedSubResource, unsigned int subResource = 0, unsigned int mapFlags = 0);
	void Unmap(ID3D11Buffer* buffer, unsigned int subResource = 0);
	void CopyResource(ID3D11Resource* dest, ID3D11Resource* src);

private:
	ComPtr<ID3D11DeviceContext> m_device_context;
};
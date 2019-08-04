#include "stdafx.h"

#include "Dx12PipelineState.h"
#include "Dx12HelperMethods.h"

Dx12PipelineState::Dx12PipelineState(D3D12_SHADER_BYTECODE vsByteCode, D3D12_SHADER_BYTECODE fsByteCode, 
	RasterizerSettings rasterizerSettings, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, 
	Format renderTargetFormat, Format depthStencilFormat,
	D3D12_INPUT_LAYOUT_DESC layoutDesc, ID3D12RootSignature* rootSignature)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = layoutDesc;								// Changes per shader
	psoDesc.pRootSignature = rootSignature;							// Changes per shader
	psoDesc.VS = vsByteCode;										// Changes per shader
	psoDesc.PS = fsByteCode;										// Changes per shader

	psoDesc.RasterizerState = rasterizerSettings.ToDirectX12();				// Not changed frequently
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);					// Not changed frequently
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);	// Not changed frequently
	psoDesc.SampleMask = UINT_MAX;											// Not changed frequently
	psoDesc.PrimitiveTopologyType = topologyType;							// Not changed frequently
	psoDesc.NumRenderTargets = 1;											// Not changed frequently
	psoDesc.RTVFormats[0] = renderTargetFormat.ToDirectX();					// Not changed frequently
	psoDesc.SampleDesc.Count = 1;											// Not changed frequently
	psoDesc.SampleDesc.Quality = 0;											// Not changed frequently
	psoDesc.DSVFormat = depthStencilFormat.ToDirectX();						// Not changed frequently

	DXCALL(GetDx12Device()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(m_pipeline_state.ReleaseAndGetAddressOf())));
}

Dx12PipelineState::~Dx12PipelineState()
{

}

ID3D12PipelineState* Dx12PipelineState::GetPipelineState() const
{
	return m_pipeline_state.Get();
}
#pragma once

#include "graphics/RasterizerSettings.h"
#include "graphics/BufferHelpers.h"

class Dx12PipelineState
{
public:
	Dx12PipelineState(D3D12_SHADER_BYTECODE vsByteCode, D3D12_SHADER_BYTECODE fsByteCode, RasterizerSettings rasterizerSettings, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, Format renderTargetFormat, Format depthStencilFormat,
		D3D12_INPUT_LAYOUT_DESC layoutDesc, ID3D12RootSignature* rootSignature);
	~Dx12PipelineState();

	ID3D12PipelineState* GetPipelineState() const;

private:
	ComPtr<ID3D12PipelineState> m_pipeline_state;
};
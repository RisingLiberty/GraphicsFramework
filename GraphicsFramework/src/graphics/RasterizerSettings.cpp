#include "stdafx.h"

#include "RasterizerSettings.h"

void RasterizerSettings::InitializeAsDefault()
{
	enable_depth_bias = false;
	is_discarded = false;
	polygon_mode = EFillMode::FILL;
	line_width = 1.0f;
	cull_mode = ECullMode::NONE;
	front_face_orientation = EFrontFaceOrientation::CLOCK_WISE;
	depth_bias_constant_factor = 0.0f;
	depth_bias_clamp = 0.0f;
	depth_bias_slope_factor = 0.0f;
	enable_multi_sample = false;
	enable_antialiased_line = false;
	forced_sample_count = 0;
	enable_scissor = false;
}

D3D11_RASTERIZER_DESC RasterizerSettings::ToDirectX11() const
{
	D3D11_RASTERIZER_DESC settings;

	settings.DepthClipEnable = enable_depth_clamp;
	settings.FillMode = polygon_mode.ToDirectX11();
	settings.CullMode = cull_mode.ToDirectX11();
	settings.FrontCounterClockwise = front_face_orientation.ToDirectX11();
	settings.DepthBias = (unsigned int)depth_bias_constant_factor;
	settings.DepthBiasClamp = depth_bias_clamp;
	settings.SlopeScaledDepthBias = depth_bias_slope_factor;
	settings.ScissorEnable = enable_scissor;
	settings.MultisampleEnable = enable_multi_sample;
	settings.AntialiasedLineEnable = enable_antialiased_line;

	return settings;

}

D3D12_RASTERIZER_DESC RasterizerSettings::ToDirectX12() const
{
	D3D12_RASTERIZER_DESC settings;

	settings.DepthClipEnable = enable_depth_clamp;
	settings.FillMode = polygon_mode.ToDirectX12();
	settings.CullMode = cull_mode.ToDirectX12();
	settings.FrontCounterClockwise = front_face_orientation.ToDirectX12();
	settings.DepthBias = (unsigned int)depth_bias_constant_factor;
	settings.DepthBiasClamp = depth_bias_clamp;
	settings.SlopeScaledDepthBias = depth_bias_slope_factor;
	settings.MultisampleEnable = enable_multi_sample;
	settings.AntialiasedLineEnable = enable_antialiased_line;
	settings.ForcedSampleCount = forced_sample_count;
	settings.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return settings;
}

VkPipelineRasterizationStateCreateInfo RasterizerSettings::ToVulkan() const
{
	VkPipelineRasterizationStateCreateInfo settings = {};
	settings.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	settings.depthClampEnable = enable_depth_clamp;
	settings.rasterizerDiscardEnable = is_discarded;
	settings.polygonMode = polygon_mode.ToVulkan();
	settings.lineWidth = 1.0f;
	settings.cullMode = cull_mode.ToVulkan();
	settings.frontFace = front_face_orientation.ToVulkan();
	settings.depthBiasEnable = enable_depth_bias;
	settings.depthBiasConstantFactor = depth_bias_constant_factor;
	settings.depthBiasClamp = depth_bias_clamp;
	settings.depthBiasSlopeFactor = depth_bias_slope_factor;

	return settings;
}
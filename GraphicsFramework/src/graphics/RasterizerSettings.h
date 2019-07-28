#pragma once

#include "CullMode.h"
#include "FillMode.h"
#include "FrontFaceOrientation.h"

struct RasterizerSettings
{
public:
	void InitializeAsDefault();

	D3D11_RASTERIZER_DESC ToDirectX11() const;
	D3D12_RASTERIZER_DESC ToDirectX12() const;
	VkPipelineRasterizationStateCreateInfo ToVulkan() const;

public:
	bool enable_depth_clamp;
	bool is_discarded;
	FillMode polygon_mode;
	float line_width;
	CullMode cull_mode;
	FrontFaceOrientation front_face_orientation;
	bool enable_depth_bias;
	float depth_bias_constant_factor;
	float depth_bias_clamp;
	float depth_bias_slope_factor;
	bool enable_multi_sample;
	bool enable_antialiased_line;
	unsigned int forced_sample_count;
	bool enable_scissor;
};
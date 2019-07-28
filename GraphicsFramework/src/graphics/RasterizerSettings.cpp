#include "stdafx.h"

#include "RasterizerSettings.h"

CullMode::CullMode(ECullMode enumValue) :
	enum_value(enumValue)
{

}

D3D12_CULL_MODE CullMode::ToDirectX12() const
{
	switch (enum_value)
	{
	case ECullMode::UNDEFINED:			return D3D12_CULL_MODE_NONE;
	case ECullMode::NONE:				return D3D12_CULL_MODE_NONE;
	case ECullMode::FRONT:				return D3D12_CULL_MODE_FRONT;
	case ECullMode::BACK:				return D3D12_CULL_MODE_BACK;
	case ECullMode::FRONT_AND_BACK:		ASSERT(false, "Invalid cull mode!") return D3D12_CULL_MODE_FRONT;
	}

	ASSERT(false, "Invalid cull mode!") return D3D12_CULL_MODE_FRONT;
}

VkCullModeFlagBits CullMode::ToVulkan() const
{
	switch (enum_value)
	{
	case ECullMode::UNDEFINED:			return VK_CULL_MODE_FLAG_BITS_MAX_ENUM;
	case ECullMode::NONE:				return VK_CULL_MODE_NONE;
	case ECullMode::FRONT:				return VK_CULL_MODE_FRONT_BIT;
	case ECullMode::BACK:				return VK_CULL_MODE_BACK_BIT;
	case ECullMode::FRONT_AND_BACK:		return VK_CULL_MODE_FRONT_AND_BACK;
	}

	return VK_CULL_MODE_FLAG_BITS_MAX_ENUM;
}

CullMode::operator ECullMode() const
{
	return enum_value;
}

PolygonMode::PolygonMode(EPolygonMode enumValue) :
	enum_value(enumValue)
{

}

D3D12_FILL_MODE PolygonMode::ToDirectX12() const
{
	switch (enum_value)
	{
	case EPolygonMode::UNDEFINED:	ASSERT(false, "Invalid polygon mode!"); return D3D12_FILL_MODE_SOLID;
	case EPolygonMode::FILL:		return D3D12_FILL_MODE_SOLID;
	case EPolygonMode::WIREFRAME:	return D3D12_FILL_MODE_WIREFRAME;
	case EPolygonMode::VERTEX:		ASSERT(false, "Invalid polygon mode!"); return D3D12_FILL_MODE_SOLID;
	}

	ASSERT(false, "Invalid polygon mode!"); return D3D12_FILL_MODE_SOLID;
}

VkPolygonMode PolygonMode::ToVulkan() const
{
	switch (enum_value)
	{
	case EPolygonMode::UNDEFINED:	return VK_POLYGON_MODE_MAX_ENUM;
	case EPolygonMode::FILL:		return VK_POLYGON_MODE_FILL;
	case EPolygonMode::WIREFRAME:	return VK_POLYGON_MODE_LINE;
	case EPolygonMode::VERTEX:		return VK_POLYGON_MODE_POINT;
	}

	return VK_POLYGON_MODE_MAX_ENUM;
}

PolygonMode::operator EPolygonMode() const
{
	return enum_value;
}

FrontFaceOrientation::FrontFaceOrientation(EFrontFaceOrientation enumValue) :
	enum_value(enumValue)
{

}

bool FrontFaceOrientation::ToDirectX12() const
{
	return enum_value == EFrontFaceOrientation::COUNTER_CLOCK_WISE ? true : false;
}

VkFrontFace FrontFaceOrientation::ToVulkan() const
{
	switch (enum_value)
	{
	case EFrontFaceOrientation::UNDEFINED:			return VK_FRONT_FACE_MAX_ENUM;
	case EFrontFaceOrientation::CLOCK_WISE:			return VK_FRONT_FACE_CLOCKWISE;
	case EFrontFaceOrientation::COUNTER_CLOCK_WISE: return VK_FRONT_FACE_COUNTER_CLOCKWISE;
	}

	return VK_FRONT_FACE_MAX_ENUM;
}

FrontFaceOrientation::operator EFrontFaceOrientation() const
{
	return enum_value;
}

void RasterizerSettings::InitializeAsDefault()
{
	enable_depth_bias = false;
	enable_discard = false;
	polygon_mode = EPolygonMode::FILL;
	line_width = 1.0f;
	cull_mode = ECullMode::NONE;
	front_face_orientation = EFrontFaceOrientation::CLOCK_WISE;
	enable_depth_bias = false;
	depth_bias_constant_factor = 0.0f;
	depth_bias_clamp = 0.0f;
	depth_bias_slope_factor = 0.0f;
	enable_multi_sample = false;
	enable_antialiased_line = false;
	forced_sample_count = 0;
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
	settings.rasterizerDiscardEnable = enable_discard;
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
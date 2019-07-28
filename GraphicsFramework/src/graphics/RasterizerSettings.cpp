#include "stdafx.h"

#include "RasterizerSettings.h"

CullMode::CullMode(ECullMode enumValue) :
	enum_value(enumValue)
{

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
}

VkPipelineRasterizationStateCreateInfo RasterizerSettings::ToVulkan() const
{
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = enable_depth_clamp;
	rasterizer.rasterizerDiscardEnable = enable_discard;
	rasterizer.polygonMode = polygon_mode.ToVulkan();
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = cull_mode.ToVulkan();
	rasterizer.frontFace = front_face_orientation.ToVulkan();
	rasterizer.depthBiasEnable = enable_depth_bias;
	rasterizer.depthBiasConstantFactor = depth_bias_constant_factor;
	rasterizer.depthBiasClamp = depth_bias_clamp;
	rasterizer.depthBiasSlopeFactor = depth_bias_slope_factor;

	return rasterizer;
}
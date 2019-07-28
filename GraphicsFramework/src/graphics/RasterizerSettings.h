#pragma once

enum class ECullMode
{
	UNDEFINED,
	NONE,
	FRONT,
	BACK,
	FRONT_AND_BACK,
};

class CullMode
{
public:
	CullMode(ECullMode enumValue = ECullMode::UNDEFINED);

	D3D11_CULL_MODE ToDirectX11() const;
	D3D12_CULL_MODE ToDirectX12() const;
	VkCullModeFlagBits ToVulkan() const;

	operator ECullMode() const;

private:
	ECullMode enum_value;
};

enum class EPolygonMode
{
	UNDEFINED,
	FILL,
	WIREFRAME,
	VERTEX,
};

class PolygonMode
{
public:
	PolygonMode(EPolygonMode enumValue = EPolygonMode::UNDEFINED);

	D3D11_FILL_MODE ToDirectX11() const;
	D3D12_FILL_MODE ToDirectX12() const;
	VkPolygonMode ToVulkan() const;

	operator EPolygonMode() const;

private:
	EPolygonMode enum_value;
};

enum class EFrontFaceOrientation
{
	UNDEFINED,
	CLOCK_WISE,
	COUNTER_CLOCK_WISE
};

class FrontFaceOrientation
{
public:
	FrontFaceOrientation(EFrontFaceOrientation enumValue = EFrontFaceOrientation::UNDEFINED);

	bool ToDirectX11() const;
	bool ToDirectX12() const;
	VkFrontFace ToVulkan() const;

	operator EFrontFaceOrientation() const;

private:
	EFrontFaceOrientation enum_value;
};

struct RasterizerSettings
{
public:
	void InitializeAsDefault();

	D3D11_RASTERIZER_DESC ToDirectX11() const;
	D3D12_RASTERIZER_DESC ToDirectX12() const;
	VkPipelineRasterizationStateCreateInfo ToVulkan() const;

public:
	bool enable_depth_clamp;
	bool enable_discard;
	PolygonMode polygon_mode;
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
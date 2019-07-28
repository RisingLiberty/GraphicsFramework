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

	VkFrontFace ToVulkan() const;

	operator EFrontFaceOrientation() const;

private:
	EFrontFaceOrientation enum_value;
};

struct RasterizerSettings
{
public:
	void InitializeAsDefault();

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

	VkPipelineRasterizationStateCreateInfo ToVulkan() const;
};
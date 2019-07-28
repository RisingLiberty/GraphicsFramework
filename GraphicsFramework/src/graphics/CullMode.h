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

	unsigned int ToOpenGL() const;
	D3D11_CULL_MODE ToDirectX11() const;
	D3D12_CULL_MODE ToDirectX12() const;
	VkCullModeFlagBits ToVulkan() const;

	operator ECullMode() const;

private:
	ECullMode enum_value;
};
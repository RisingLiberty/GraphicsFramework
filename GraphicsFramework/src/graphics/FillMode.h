#pragma once

enum class EFillMode
{
	UNDEFINED,
	FILL,
	WIREFRAME,
	VERTEX,
};

class FillMode
{
public:
	FillMode(EFillMode enumValue = EFillMode::UNDEFINED);

	unsigned int ToOpenGL() const;
	D3D11_FILL_MODE ToDirectX11() const;
	D3D12_FILL_MODE ToDirectX12() const;
	VkPolygonMode ToVulkan() const;

	operator EFillMode() const;

private:
	EFillMode enum_value;
};


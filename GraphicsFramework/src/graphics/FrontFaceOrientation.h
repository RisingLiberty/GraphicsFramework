#pragma once

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

	unsigned int ToOpenGL() const;
	bool ToDirectX11() const;
	bool ToDirectX12() const;
	VkFrontFace ToVulkan() const;

	operator EFrontFaceOrientation() const;

private:
	EFrontFaceOrientation enum_value;
};


#include "stdafx.h"

#include "FrontFaceOrientation.h"

FrontFaceOrientation::FrontFaceOrientation(EFrontFaceOrientation enumValue) :
	enum_value(enumValue)
{

}

unsigned int FrontFaceOrientation::ToOpenGL() const
{
	return enum_value == EFrontFaceOrientation::COUNTER_CLOCK_WISE ? GL_CCW : GL_CW;
}

bool FrontFaceOrientation::ToDirectX11() const
{
	return enum_value == EFrontFaceOrientation::COUNTER_CLOCK_WISE ? true : false;
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

#include "stdafx.h"

#include "FillMode.h"

FillMode::FillMode(EFillMode enumValue) :
	enum_value(enumValue)
{

}

unsigned int FillMode::ToOpenGL() const
{
	switch (enum_value)
	{
	case EFillMode::UNDEFINED:		return -1;
	case EFillMode::FILL:			return GL_FILL;
	case EFillMode::WIREFRAME:		return GL_LINE;
	case EFillMode::VERTEX:			return GL_POINT;
	}

	return -1;
}

D3D11_FILL_MODE FillMode::ToDirectX11() const
{
	switch (enum_value)
	{
	case EFillMode::UNDEFINED:	ASSERT(false, "Invalid polygon mode!"); return D3D11_FILL_SOLID;
	case EFillMode::FILL:		return D3D11_FILL_SOLID;
	case EFillMode::WIREFRAME:	return D3D11_FILL_WIREFRAME;
	case EFillMode::VERTEX:		ASSERT(false, "Invalid polygon mode!"); return D3D11_FILL_SOLID;
	}

	ASSERT(false, "Invalid polygon mode!"); return D3D11_FILL_SOLID;
}

D3D12_FILL_MODE FillMode::ToDirectX12() const
{
	switch (enum_value)
	{
	case EFillMode::UNDEFINED:	ASSERT(false, "Invalid polygon mode!"); return D3D12_FILL_MODE_SOLID;
	case EFillMode::FILL:		return D3D12_FILL_MODE_SOLID;
	case EFillMode::WIREFRAME:	return D3D12_FILL_MODE_WIREFRAME;
	case EFillMode::VERTEX:		ASSERT(false, "Invalid polygon mode!"); return D3D12_FILL_MODE_SOLID;
	}

	ASSERT(false, "Invalid polygon mode!"); return D3D12_FILL_MODE_SOLID;
}

VkPolygonMode FillMode::ToVulkan() const
{
	switch (enum_value)
	{
	case EFillMode::UNDEFINED:	return VK_POLYGON_MODE_MAX_ENUM;
	case EFillMode::FILL:		return VK_POLYGON_MODE_FILL;
	case EFillMode::WIREFRAME:	return VK_POLYGON_MODE_LINE;
	case EFillMode::VERTEX:		return VK_POLYGON_MODE_POINT;
	}

	return VK_POLYGON_MODE_MAX_ENUM;
}

FillMode::operator EFillMode() const
{
	return enum_value;
}

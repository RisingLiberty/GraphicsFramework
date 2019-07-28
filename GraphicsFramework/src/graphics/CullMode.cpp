#include "stdafx.h"

#include "CullMode.h"

CullMode::CullMode(ECullMode enumValue) :
	enum_value(enumValue)
{

}

unsigned int CullMode::ToOpenGL() const
{
	switch (enum_value)
	{
	case ECullMode::UNDEFINED:			return -1;
	case ECullMode::NONE:				return -1;
	case ECullMode::FRONT:				return GL_FRONT;
	case ECullMode::BACK:				return GL_BACK;
	case ECullMode::FRONT_AND_BACK:		return GL_FRONT_AND_BACK;
	}

	return -1;
}

D3D11_CULL_MODE CullMode::ToDirectX11() const
{
	switch (enum_value)
	{
	case ECullMode::UNDEFINED:			return D3D11_CULL_NONE;
	case ECullMode::NONE:				return D3D11_CULL_NONE;
	case ECullMode::FRONT:				return D3D11_CULL_FRONT;
	case ECullMode::BACK:				return D3D11_CULL_BACK;
	case ECullMode::FRONT_AND_BACK:		ASSERT(false, "Invalid cull mode!") return D3D11_CULL_FRONT;
	}

	ASSERT(false, "Invalid cull mode!") return D3D11_CULL_FRONT;
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

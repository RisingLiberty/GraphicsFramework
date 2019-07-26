#include "stdafx.h"#include "BufferHelpers.h"Format::Format(EFormat enumValue) :	enum_value(enumValue){}DXGI_FORMAT Format::ToDirectX() const{	switch (enum_value)	{	case EFormat::UNKNOWN:										return DXGI_FORMAT_UNKNOWN;	case EFormat::R32G32B32A32_TYPLESS:							return DXGI_FORMAT_R32G32B32A32_TYPELESS;	case EFormat::R32G32B32A32_FLOAT:							return DXGI_FORMAT_R32G32B32A32_FLOAT;	case EFormat::R32G32B32A32_UINT:							return DXGI_FORMAT_R32G32B32A32_UINT;	case EFormat::R32G32B32A32_SINT:							return DXGI_FORMAT_R32G32B32A32_SINT;	case EFormat::R32G32B32_TYPELESS:							return DXGI_FORMAT_R32G32B32_TYPELESS;	case EFormat::R32G32B32_FLOAT:								return DXGI_FORMAT_R32G32B32_FLOAT;	case EFormat::R32G32B32_UINT:								return DXGI_FORMAT_R32G32B32_UINT;	case EFormat::R32G32B32_SINT:								return DXGI_FORMAT_R32G32B32_SINT;	case EFormat::R16G16B16A16_TYPELESS:						return DXGI_FORMAT_R16G16B16A16_TYPELESS;	case EFormat::R16G16B16A16_FLOAT:							return DXGI_FORMAT_R16G16B16A16_FLOAT;	case EFormat::R16G16B16A16_UNORM:							return DXGI_FORMAT_R16G16B16A16_UNORM;	case EFormat::R16G16B16A16_UINT:							return DXGI_FORMAT_R16G16B16A16_UINT;	case EFormat::R16G16B16A16_SNORM:							return DXGI_FORMAT_R16G16B16A16_SNORM;	case EFormat::R16G16B16A16_SINT:							return DXGI_FORMAT_R16G16B16A16_SINT;	case EFormat::R32G32_TYPELESS:								return DXGI_FORMAT_R32G32_TYPELESS;	case EFormat::R32G32_FLOAT:									return DXGI_FORMAT_R32G32_FLOAT;	case EFormat::R32G32_UINT:									return DXGI_FORMAT_R32G32_UINT;	case EFormat::R32G32_SINT:									return DXGI_FORMAT_R32G32_SINT;	case EFormat::R32G8X24_TYPELESS:							return DXGI_FORMAT_R32G8X24_TYPELESS;	case EFormat::D32_FLOAT_S8X24_UINT:							return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;	case EFormat::R32_FLOAT_X8X24_TYPELESS:						return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;	case EFormat::X32_TYPELESS_G8X24_UINT:						return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;	case EFormat::R10G10B10A2_TYPELESS:							return DXGI_FORMAT_R10G10B10A2_TYPELESS;	case EFormat::R10G10B10A2_UNORM:							return DXGI_FORMAT_R10G10B10A2_UNORM;	case EFormat::R10G10B10A2_UINT:								return DXGI_FORMAT_R10G10B10A2_UINT;	case EFormat::R11G11B10_FLOAT:								return DXGI_FORMAT_R11G11B10_FLOAT;	case EFormat::R8G8B8A8_TYPELESS:							return DXGI_FORMAT_R8G8B8A8_TYPELESS;	case EFormat::R8G8B8A8_UNORM:								return DXGI_FORMAT_R8G8B8A8_UNORM;	case EFormat::R8G8B8A8_UNORM_SRGB:							return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	case EFormat::R8G8B8A8_UINT:								return DXGI_FORMAT_R8G8B8A8_UINT;	case EFormat::R8G8B8A8_SNORM:								return DXGI_FORMAT_R8G8B8A8_SNORM;	case EFormat::R8G8B8A8_SINT:								return DXGI_FORMAT_R8G8B8A8_SINT;	case EFormat::R16G16_TYPELESS:								return DXGI_FORMAT_R16G16_TYPELESS;	case EFormat::R16G16_FLOAT:									return DXGI_FORMAT_R16G16_FLOAT;	case EFormat::R16G16_UNORM:									return DXGI_FORMAT_R16G16_UNORM;	case EFormat::R16G16_UINT:									return DXGI_FORMAT_R16G16_UINT;	case EFormat::R16G16_SNORM:									return DXGI_FORMAT_R16G16_SNORM;	case EFormat::R16G16_SINT:									return DXGI_FORMAT_R16G16_SINT;	case EFormat::R32_TYPELESS:									return DXGI_FORMAT_R32_TYPELESS;	case EFormat::D32_FLOAT:									return DXGI_FORMAT_D32_FLOAT;	case EFormat::R32_FLOAT:									return DXGI_FORMAT_R32_FLOAT;	case EFormat::R32_UINT:										return DXGI_FORMAT_R32_UINT;	case EFormat::R32_SINT:										return DXGI_FORMAT_R32_SINT;	case EFormat::R24G8_TYPELESS:								return DXGI_FORMAT_R24G8_TYPELESS;	case EFormat::D24_UNORM_S8_UINT:							return DXGI_FORMAT_D24_UNORM_S8_UINT;	case EFormat::R24_UNORM_X8_TYPELESS:						return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;	case EFormat::X24_TYPELESS_G8_UINT:							return DXGI_FORMAT_X24_TYPELESS_G8_UINT;	case EFormat::R8G8_TYPELESS:								return DXGI_FORMAT_R8G8_TYPELESS;	case EFormat::R8G8_UNORM:									return DXGI_FORMAT_R8G8_UNORM;	case EFormat::R8G8_UINT:									return DXGI_FORMAT_R8G8_UINT;	case EFormat::R8G8_SNORM:									return DXGI_FORMAT_R8G8_SNORM;	case EFormat::R8G8_SINT:									return DXGI_FORMAT_R8G8_SINT;	case EFormat::R16_TYPELESS:									return DXGI_FORMAT_R16_TYPELESS;	case EFormat::R16_FLOAT:									return DXGI_FORMAT_R16_FLOAT;	case EFormat::D16_UNORM:									return DXGI_FORMAT_D16_UNORM;	case EFormat::R16_UNORM:									return DXGI_FORMAT_R16_UNORM;	case EFormat::R16_UINT:										return DXGI_FORMAT_R16_UINT;	case EFormat::R16_SNORM:									return DXGI_FORMAT_R16_SNORM;	case EFormat::R16_SINT:										return DXGI_FORMAT_R16_SINT;	case EFormat::R8_TYPELESS:									return DXGI_FORMAT_R8_TYPELESS;	case EFormat::R8_UNORM:										return DXGI_FORMAT_R8_UNORM;	case EFormat::R8_UINT:										return DXGI_FORMAT_R8_UINT;	case EFormat::R8_SNORM:										return DXGI_FORMAT_R8_SNORM;	case EFormat::R8_SINT:										return DXGI_FORMAT_R8_SINT;	case EFormat::A8_UNORM:										return DXGI_FORMAT_A8_UNORM;	case EFormat::R1_UNORM:										return DXGI_FORMAT_R1_UNORM;	case EFormat::R9G9B9E5_SHAREDEXP:							return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;	case EFormat::R8G8_B8G8_UNORM:								return DXGI_FORMAT_R8G8_B8G8_UNORM;	case EFormat::G8R8_G8B8_UNORM:								return DXGI_FORMAT_G8R8_G8B8_UNORM;	case EFormat::BC1_TYPELESS:									return DXGI_FORMAT_BC1_TYPELESS;	case EFormat::BC1_UNORM:									return DXGI_FORMAT_BC1_UNORM;	case EFormat::BC1_UNORM_SRGB:								return DXGI_FORMAT_BC1_UNORM_SRGB;	case EFormat::BC2_TYPELESS:									return DXGI_FORMAT_BC2_TYPELESS;	case EFormat::BC2_UNORM:									return DXGI_FORMAT_BC2_UNORM;	case EFormat::BC2_UNORM_SRGB:								return DXGI_FORMAT_BC2_UNORM_SRGB;	case EFormat::BC3_TYPELESS:									return DXGI_FORMAT_BC3_TYPELESS;	case EFormat::BC3_UNORM:									return DXGI_FORMAT_BC3_UNORM;	case EFormat::BC3_UNORM_SRGB:								return DXGI_FORMAT_BC3_UNORM_SRGB;	case EFormat::BC4_TYPELESS:									return DXGI_FORMAT_BC4_TYPELESS;	case EFormat::BC4_UNORM:									return DXGI_FORMAT_BC4_UNORM;	case EFormat::BC4_SNORM:									return DXGI_FORMAT_BC4_SNORM;	case EFormat::BC5_TYPELESS:									return DXGI_FORMAT_BC5_TYPELESS;	case EFormat::BC5_UNORM:									return DXGI_FORMAT_BC5_UNORM;	case EFormat::BC5_SNORM:									return DXGI_FORMAT_BC5_SNORM;	case EFormat::B5G6R5_UNORM:									return DXGI_FORMAT_B5G6R5_UNORM;	case EFormat::B5G5R5A1_UNORM:								return DXGI_FORMAT_B5G5R5A1_UNORM;	case EFormat::B8G8R8A8_UNORM:								return DXGI_FORMAT_B8G8R8A8_UNORM;	case EFormat::B8G8R8X8_UNORM:								return DXGI_FORMAT_B8G8R8X8_UNORM;	case EFormat::R10G10B10_XR_BIAS_A2_UNORM:					return DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;	case EFormat::B8G8R8A8_TYPELESS:							return DXGI_FORMAT_B8G8R8A8_TYPELESS;	case EFormat::B8G8R8A8_UNORM_SRGB:							return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;	case EFormat::B8G8R8X8_TYPELESS:							return DXGI_FORMAT_B8G8R8X8_TYPELESS;	case EFormat::B8G8R8X8_UNORM_SRGB:							return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;	case EFormat::B4G4R4A4_UNORM:								return DXGI_FORMAT_B4G4R4A4_UNORM;	}	return DXGI_FORMAT_UNKNOWN;}unsigned int Format::ToOpenGL() const{	switch (enum_value)
	{
	case EFormat::UNKNOWN:										  return -1;
	case EFormat::R32G32B32A32_TYPLESS:						  return -1;
	case EFormat::R32G32B32A32_FLOAT:							  return GL_FLOAT_VEC4;
	case EFormat::R32G32B32A32_UINT:							  return GL_UNSIGNED_INT_VEC4;
	case EFormat::R32G32B32A32_SINT:							  return GL_INT_VEC4;
	case EFormat::R32G32B32_TYPELESS:							  return -1;
	case EFormat::R32G32B32_FLOAT:								  return GL_FLOAT_VEC3;
	case EFormat::R32G32B32_UINT:								  return GL_UNSIGNED_INT_VEC3;
	case EFormat::R32G32B32_SINT:								  return GL_INT_VEC3;
	case EFormat::R16G16B16A16_TYPELESS:						  return -1;
	case EFormat::R16G16B16A16_FLOAT:							  return -1;
	case EFormat::R16G16B16A16_UNORM:							  return -1;
	case EFormat::R16G16B16A16_UINT:							  return -1;
	case EFormat::R16G16B16A16_SNORM:							  return -1;
	case EFormat::R16G16B16A16_SINT:							  return -1;
	case EFormat::R32G32_TYPELESS:								  return -1;
	case EFormat::R32G32_FLOAT:								  return GL_FLOAT_VEC2;
	case EFormat::R32G32_UINT:									  return GL_UNSIGNED_INT_VEC2;
	case EFormat::R32G32_SINT:									  return GL_INT_VEC2;
	case EFormat::R32G8X24_TYPELESS:							  return -1;
	case EFormat::D32_FLOAT_S8X24_UINT:						  return -1;
	case EFormat::R32_FLOAT_X8X24_TYPELESS:					  return -1;
	case EFormat::X32_TYPELESS_G8X24_UINT:						  return -1;
	case EFormat::R10G10B10A2_TYPELESS:						  return -1;
	case EFormat::R10G10B10A2_UNORM:							  return -1;
	case EFormat::R10G10B10A2_UINT:							  return -1;
	case EFormat::R11G11B10_FLOAT:								  return -1;
	case EFormat::R8G8B8A8_TYPELESS:							  return -1;
	case EFormat::R8G8B8A8_UNORM:								  return -1;
	case EFormat::R8G8B8A8_UNORM_SRGB:							  return -1;
	case EFormat::R8G8B8A8_UINT:								  return -1;
	case EFormat::R8G8B8A8_SNORM:								  return -1;
	case EFormat::R8G8B8A8_SINT:								  return -1;
	case EFormat::R16G16_TYPELESS:								  return -1;
	case EFormat::R16G16_FLOAT:								  return -1;
	case EFormat::R16G16_UNORM:								  return -1;
	case EFormat::R16G16_UINT:									  return -1;
	case EFormat::R16G16_SNORM:								  return -1;
	case EFormat::R16G16_SINT:									  return -1;
	case EFormat::R32_TYPELESS:								  return -1;
	case EFormat::D32_FLOAT:									  return GL_FLOAT;
	case EFormat::R32_FLOAT:									  return GL_FLOAT;
	case EFormat::R32_UINT:									  return GL_UNSIGNED_INT;
	case EFormat::R32_SINT:									  return GL_INT;
	case EFormat::R24G8_TYPELESS:								  return -1;
	case EFormat::D24_UNORM_S8_UINT:							  return GL_DEPTH24_STENCIL8;
	case EFormat::R24_UNORM_X8_TYPELESS:						  return -1;
	case EFormat::X24_TYPELESS_G8_UINT:						  return -1;
	case EFormat::R8G8_TYPELESS:								  return -1;
	case EFormat::R8G8_UNORM:									  return -1;
	case EFormat::R8G8_UINT:									  return -1;
	case EFormat::R8G8_SNORM:									  return -1;
	case EFormat::R8G8_SINT:									  return -1;
	case EFormat::R16_TYPELESS:								  return -1;
	case EFormat::R16_FLOAT:									  return -1;
	case EFormat::D16_UNORM:									  return -1;
	case EFormat::R16_UNORM:									  return -1;
	case EFormat::R16_UINT:									  return -1;
	case EFormat::R16_SNORM:									  return -1;
	case EFormat::R16_SINT:									  return -1;
	case EFormat::R8_TYPELESS:									  return -1;
	case EFormat::R8_UNORM:									  return GL_UNSIGNED_BYTE;
	case EFormat::R8_UINT:										  return GL_UNSIGNED_BYTE;
	case EFormat::R8_SNORM:									  return GL_BYTE;
	case EFormat::R8_SINT:										  return GL_BYTE;
	case EFormat::A8_UNORM:									  return -1;
	case EFormat::R1_UNORM:									  return -1;
	case EFormat::R9G9B9E5_SHAREDEXP:							  return -1;
	case EFormat::R8G8_B8G8_UNORM:								  return -1;
	case EFormat::G8R8_G8B8_UNORM:								  return -1;
	case EFormat::BC1_TYPELESS:								  return -1;
	case EFormat::BC1_UNORM:									  return -1;
	case EFormat::BC1_UNORM_SRGB:								  return -1;
	case EFormat::BC2_TYPELESS:								  return -1;
	case EFormat::BC2_UNORM:									  return -1;
	case EFormat::BC2_UNORM_SRGB:								  return -1;
	case EFormat::BC3_TYPELESS:								  return -1;
	case EFormat::BC3_UNORM:									  return -1;
	case EFormat::BC3_UNORM_SRGB:								  return -1;
	case EFormat::BC4_TYPELESS:								  return -1;
	case EFormat::BC4_UNORM:									  return -1;
	case EFormat::BC4_SNORM:									  return -1;
	case EFormat::BC5_TYPELESS:								  return -1;
	case EFormat::BC5_UNORM:									  return -1;
	case EFormat::BC5_SNORM:									  return -1;
	case EFormat::B5G6R5_UNORM:								  return -1;
	case EFormat::B5G5R5A1_UNORM:								  return -1;
	case EFormat::B8G8R8A8_UNORM:								  return -1;
	case EFormat::B8G8R8X8_UNORM:								  return -1;
	case EFormat::R10G10B10_XR_BIAS_A2_UNORM:					  return -1;
	case EFormat::B8G8R8A8_TYPELESS:							  return -1;
	case EFormat::B8G8R8A8_UNORM_SRGB:							  return -1;
	case EFormat::B8G8R8X8_TYPELESS:							  return -1;
	case EFormat::B8G8R8X8_UNORM_SRGB:							  return -1;
	case EFormat::B4G4R4A4_UNORM:								  return -1;
	}	return -1;}VkFormat Format::ToVulkan() const{	switch (enum_value)
	{
	case EFormat::UNKNOWN:							return VK_FORMAT_UNDEFINED;
	case EFormat::R32G32B32A32_TYPLESS:				return VK_FORMAT_UNDEFINED;
	case EFormat::R32G32B32A32_FLOAT:				return VK_FORMAT_R32G32B32A32_SFLOAT;
	case EFormat::R32G32B32A32_UINT:				return VK_FORMAT_R32G32B32A32_UINT;
	case EFormat::R32G32B32A32_SINT:				return VK_FORMAT_R32G32B32A32_SINT;
	case EFormat::R32G32B32_TYPELESS:				return VK_FORMAT_UNDEFINED;
	case EFormat::R32G32B32_FLOAT:					return VK_FORMAT_R32G32_SFLOAT;
	case EFormat::R32G32B32_UINT:					return VK_FORMAT_R32G32B32_UINT;
	case EFormat::R32G32B32_SINT:					return VK_FORMAT_R32G32B32_SINT;
	case EFormat::R16G16B16A16_TYPELESS:			return VK_FORMAT_UNDEFINED;
	case EFormat::R16G16B16A16_FLOAT:				return VK_FORMAT_R16G16B16A16_SFLOAT;
	case EFormat::R16G16B16A16_UNORM:				return VK_FORMAT_R16G16B16A16_UNORM;
	case EFormat::R16G16B16A16_UINT:				return VK_FORMAT_R16G16B16A16_UINT;
	case EFormat::R16G16B16A16_SNORM:				return VK_FORMAT_R16G16B16A16_SNORM;
	case EFormat::R16G16B16A16_SINT:				return VK_FORMAT_R16G16B16A16_SINT;
	case EFormat::R32G32_TYPELESS:					return VK_FORMAT_UNDEFINED;
	case EFormat::R32G32_FLOAT:						return VK_FORMAT_R32G32_SFLOAT;
	case EFormat::R32G32_UINT:						return VK_FORMAT_R32G32_UINT;
	case EFormat::R32G32_SINT:						return VK_FORMAT_R32G32_SINT;
	case EFormat::R32G8X24_TYPELESS:				return VK_FORMAT_UNDEFINED;
	case EFormat::D32_FLOAT_S8X24_UINT:				return VK_FORMAT_UNDEFINED;
	case EFormat::R32_FLOAT_X8X24_TYPELESS:			return VK_FORMAT_UNDEFINED;
	case EFormat::X32_TYPELESS_G8X24_UINT:			return VK_FORMAT_UNDEFINED;
	case EFormat::R10G10B10A2_TYPELESS:				return VK_FORMAT_UNDEFINED;
	case EFormat::R10G10B10A2_UNORM:				return VK_FORMAT_UNDEFINED;
	case EFormat::R10G10B10A2_UINT:					return VK_FORMAT_UNDEFINED;
	case EFormat::R11G11B10_FLOAT:					return VK_FORMAT_UNDEFINED;
	case EFormat::R8G8B8A8_TYPELESS:				return VK_FORMAT_UNDEFINED;
	case EFormat::R8G8B8A8_UNORM:					return VK_FORMAT_R8G8B8A8_UNORM;
	case EFormat::R8G8B8A8_UNORM_SRGB:				return VK_FORMAT_R8G8B8A8_SRGB;
	case EFormat::R8G8B8A8_UINT:					return VK_FORMAT_R8G8B8A8_UINT;
	case EFormat::R8G8B8A8_SNORM:					return VK_FORMAT_R8G8B8A8_SNORM;
	case EFormat::R8G8B8A8_SINT:					return VK_FORMAT_R8G8B8A8_SINT;
	case EFormat::R16G16_TYPELESS:					return VK_FORMAT_UNDEFINED;
	case EFormat::R16G16_FLOAT:						return VK_FORMAT_R16G16_SFLOAT;
	case EFormat::R16G16_UNORM:						return VK_FORMAT_R16G16_UNORM;
	case EFormat::R16G16_UINT:						return VK_FORMAT_R16G16_UINT;
	case EFormat::R16G16_SNORM:						return VK_FORMAT_R16G16_SNORM;
	case EFormat::R16G16_SINT:						return VK_FORMAT_R16G16_SINT;
	case EFormat::R32_TYPELESS:						return VK_FORMAT_UNDEFINED;
	case EFormat::D32_FLOAT:						return VK_FORMAT_D32_SFLOAT;
	case EFormat::R32_FLOAT:						return VK_FORMAT_R32_SFLOAT;
	case EFormat::R32_UINT:							return VK_FORMAT_R32_UINT;
	case EFormat::R32_SINT:							return VK_FORMAT_R32_SINT;
	case EFormat::R24G8_TYPELESS:					return VK_FORMAT_UNDEFINED;
	case EFormat::D24_UNORM_S8_UINT:				return VK_FORMAT_X8_D24_UNORM_PACK32;
	case EFormat::R24_UNORM_X8_TYPELESS:			return VK_FORMAT_UNDEFINED;
	case EFormat::X24_TYPELESS_G8_UINT:				return VK_FORMAT_UNDEFINED;
	case EFormat::R8G8_TYPELESS:					return VK_FORMAT_UNDEFINED;
	case EFormat::R8G8_UNORM:						return VK_FORMAT_R8G8_UNORM;
	case EFormat::R8G8_UINT:						return VK_FORMAT_R8G8_UINT;
	case EFormat::R8G8_SNORM:						return VK_FORMAT_R8G8_SNORM;
	case EFormat::R8G8_SINT:						return VK_FORMAT_R8G8_SINT;
	case EFormat::R16_TYPELESS:						return VK_FORMAT_UNDEFINED;
	case EFormat::R16_FLOAT:						return VK_FORMAT_R16_SFLOAT;
	case EFormat::D16_UNORM:						return VK_FORMAT_D16_UNORM;
	case EFormat::R16_UNORM:						return VK_FORMAT_R16_UNORM;
	case EFormat::R16_UINT:							return VK_FORMAT_R16_UINT;
	case EFormat::R16_SNORM:						return VK_FORMAT_R16_SNORM;
	case EFormat::R16_SINT:							return VK_FORMAT_R16_SINT;
	case EFormat::R8_TYPELESS:						return VK_FORMAT_UNDEFINED;
	case EFormat::R8_UNORM:							return VK_FORMAT_R8_UNORM;
	case EFormat::R8_UINT:							return VK_FORMAT_R8_UINT;
	case EFormat::R8_SNORM:							return VK_FORMAT_R8_SNORM;
	case EFormat::R8_SINT:							return VK_FORMAT_R8_SINT;
	case EFormat::A8_UNORM:							return VK_FORMAT_R8_UNORM;
	case EFormat::R1_UNORM:							return VK_FORMAT_UNDEFINED;
	case EFormat::R9G9B9E5_SHAREDEXP:				return VK_FORMAT_UNDEFINED;
	case EFormat::R8G8_B8G8_UNORM:					return VK_FORMAT_UNDEFINED;
	case EFormat::G8R8_G8B8_UNORM:					return VK_FORMAT_UNDEFINED;
	case EFormat::BC1_TYPELESS:						return VK_FORMAT_UNDEFINED;
	case EFormat::BC1_UNORM:						return VK_FORMAT_UNDEFINED;
	case EFormat::BC1_UNORM_SRGB:					return VK_FORMAT_UNDEFINED;
	case EFormat::BC2_TYPELESS:						return VK_FORMAT_UNDEFINED;
	case EFormat::BC2_UNORM:						return VK_FORMAT_UNDEFINED;
	case EFormat::BC2_UNORM_SRGB:					return VK_FORMAT_UNDEFINED;
	case EFormat::BC3_TYPELESS:						return VK_FORMAT_UNDEFINED;
	case EFormat::BC3_UNORM:						return VK_FORMAT_UNDEFINED;
	case EFormat::BC3_UNORM_SRGB:					return VK_FORMAT_UNDEFINED;
	case EFormat::BC4_TYPELESS:						return VK_FORMAT_UNDEFINED;
	case EFormat::BC4_UNORM:						return VK_FORMAT_UNDEFINED;
	case EFormat::BC4_SNORM:						return VK_FORMAT_UNDEFINED;
	case EFormat::BC5_TYPELESS:						return VK_FORMAT_UNDEFINED;
	case EFormat::BC5_UNORM:						return VK_FORMAT_UNDEFINED;
	case EFormat::BC5_SNORM:						return VK_FORMAT_UNDEFINED;
	case EFormat::B5G6R5_UNORM:						return VK_FORMAT_UNDEFINED;
	case EFormat::B5G5R5A1_UNORM:					return VK_FORMAT_UNDEFINED;
	case EFormat::B8G8R8A8_UNORM:					return VK_FORMAT_UNDEFINED;
	case EFormat::B8G8R8X8_UNORM:					return VK_FORMAT_UNDEFINED;
	case EFormat::R10G10B10_XR_BIAS_A2_UNORM:		return VK_FORMAT_UNDEFINED;
	case EFormat::B8G8R8A8_TYPELESS:				return VK_FORMAT_UNDEFINED;
	case EFormat::B8G8R8A8_UNORM_SRGB:				return VK_FORMAT_UNDEFINED;
	case EFormat::B8G8R8X8_TYPELESS:				return VK_FORMAT_UNDEFINED;
	case EFormat::B8G8R8X8_UNORM_SRGB:				return VK_FORMAT_UNDEFINED;
	case EFormat::B4G4R4A4_UNORM:					return VK_FORMAT_UNDEFINED;	};	return VK_FORMAT_UNDEFINED;}

Format::operator EFormat() const
{
	return enum_value;}Topology::Topology(ETopology enumValue) :	enum_value(enumValue){}D3D_PRIMITIVE_TOPOLOGY Topology::ToDirectX() const{
	switch (enum_value)
	{
	case ETopology::UNDEFINED:				return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
	case ETopology::POINTLIST:				return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
	case ETopology::LINELIST:				return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	case ETopology::LINESTRIP:				return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
	case ETopology::TRIANGLELIST:			return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case ETopology::TRIANGLESTRIP:			return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	case ETopology::LINELIST_ADJ:			return D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
	case ETopology::LINESTRIP_ADJ:			return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
	case ETopology::TRIANGLELIST_ADJ:		return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
	case ETopology::TRIANGLESTRIP_ADJ:		return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
	}	return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;}unsigned int Topology::ToOpenGL() const{
	switch (enum_value)
	{
	case ETopology::UNDEFINED:				return -1;
	case ETopology::POINTLIST:				return GL_POINT;
	case ETopology::LINELIST:				return GL_LINES;
	case ETopology::LINESTRIP:				return GL_LINE_STRIP;
	case ETopology::TRIANGLELIST:			return GL_TRIANGLES;
	case ETopology::TRIANGLESTRIP:			return GL_TRIANGLE_STRIP;
	case ETopology::LINELIST_ADJ:			return GL_LINES_ADJACENCY;
	case ETopology::LINESTRIP_ADJ:			return GL_LINE_STRIP_ADJACENCY;
	case ETopology::TRIANGLELIST_ADJ:		return GL_TRIANGLES_ADJACENCY;
	case ETopology::TRIANGLESTRIP_ADJ:		return GL_TRIANGLE_STRIP_ADJACENCY;
	}	return -1;}VkPrimitiveTopology Topology::ToVulkan() const{
	switch (enum_value)
	{
	case ETopology::UNDEFINED:				return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
	case ETopology::POINTLIST:				return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
	case ETopology::LINELIST:				return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	case ETopology::LINESTRIP:				return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
	case ETopology::TRIANGLELIST:			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	case ETopology::TRIANGLESTRIP:			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	case ETopology::LINELIST_ADJ:			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
	case ETopology::LINESTRIP_ADJ:			return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
	case ETopology::TRIANGLELIST_ADJ:		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
	case ETopology::TRIANGLESTRIP_ADJ:		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
	}	return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;}
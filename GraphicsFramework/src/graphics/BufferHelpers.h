#pragma once

enum class BufferType
{
	VERTEX,
	INDEX,
	COPY_SOURCE,
	COPY_DESTINATION
};

enum class EBufferAccess
{
	UNDEFINED	= -1,
	NO_ACCESS	= 0,
	READ		= 1,
	WRITE		= 2,
	PERSISTENT	= 4,
	COHERENT	= 8,
	COUNT		= COHERENT / 2
};

struct BufferAccess
{
public:
	BufferAccess(EBufferAccess enumValue = EBufferAccess::NO_ACCESS);

	unsigned int ToOpenGL() const;

	operator EBufferAccess() const;

public:
	EBufferAccess enum_value;
};

enum class EBufferStage
{
	UNDEFINED								= (1 << 0),
	COPY_SRC								= (1 << 1),
	COPY_DEST								= (1 << 2),
	UNIFORM_TEXEL							= (1 << 3),
	STORGAGE_TEXEL							= (1 << 4),
	UNIFORM_BUFFER							= (1 << 5),
	STORGAGE_BUFFER							= (1 << 6),
	INDEX_BUFFER							= (1 << 7),
	VERTEX_BUFFER							= (1 << 8),
	INDIRECT_BUFFER							= (1 << 9),
	TRANSFORM_FEEDBACK_BUFFER				= (1 << 10),
	TRANSFORM_FEEDBACK_COUNTER_BUFFER		= (1 << 11),
	CONDITIONAL_RENDERING					= (1 << 12),
	RAY_TRACING								= (1 << 13),
	SHADER_DEVICE_ADDRESS					= (1 << 14)
};

struct BufferStage
{
public:
	BufferStage(EBufferStage enumValue = EBufferStage::UNDEFINED);

	D3D12_RESOURCE_STATES ToDirectX12() const;
	VkBufferUsageFlagBits ToVulkan() const;

	operator EBufferStage() const;

public:
	EBufferStage enum_value;
};

enum class EBufferUsage
{
	DYNAMIC,
	STATIC
};

struct BufferUsage
{
public:
	BufferUsage(EBufferUsage enumValue);

	unsigned int ToOpenGL() const;
	D3D11_USAGE ToDirectX11() const;

	operator EBufferUsage() const;

public:
	EBufferUsage enum_value;
};

// These are directly mapped to direct x
// and will probably change over time
enum class EFormat
{
	UNKNOWN,
	R32G32B32A32_TYPLESS,
	R32G32B32A32_FLOAT,
	R32G32B32A32_UINT,
	R32G32B32A32_SINT,
	R32G32B32_TYPELESS,
	R32G32B32_FLOAT,
	R32G32B32_UINT,
	R32G32B32_SINT,
	R16G16B16A16_TYPELESS,
	R16G16B16A16_FLOAT,
	R16G16B16A16_UNORM,
	R16G16B16A16_UINT,
	R16G16B16A16_SNORM,
	R16G16B16A16_SINT,
	R32G32_TYPELESS,
	R32G32_FLOAT,
	R32G32_UINT,
	R32G32_SINT,
	R32G8X24_TYPELESS,
	D32_FLOAT_S8X24_UINT,
	R32_FLOAT_X8X24_TYPELESS,
	X32_TYPELESS_G8X24_UINT,
	R10G10B10A2_TYPELESS,
	R10G10B10A2_UNORM,
	R10G10B10A2_UINT,
	R11G11B10_FLOAT,
	R8G8B8A8_TYPELESS,
	R8G8B8A8_UNORM,
	R8G8B8A8_UNORM_SRGB,
	R8G8B8A8_UINT,
	R8G8B8A8_SNORM,
	R8G8B8A8_SINT,
	R16G16_TYPELESS,
	R16G16_FLOAT,
	R16G16_UNORM,
	R16G16_UINT,
	R16G16_SNORM,
	R16G16_SINT,
	R32_TYPELESS,
	D32_FLOAT,
	R32_FLOAT,
	R32_UINT,
	R32_SINT,
	R24G8_TYPELESS,
	D24_UNORM_S8_UINT,
	R24_UNORM_X8_TYPELESS,
	X24_TYPELESS_G8_UINT,
	R8G8_TYPELESS,
	R8G8_UNORM,
	R8G8_UINT,
	R8G8_SNORM,
	R8G8_SINT,
	R16_TYPELESS,
	R16_FLOAT,
	D16_UNORM,
	R16_UNORM,
	R16_UINT,
	R16_SNORM,
	R16_SINT,
	R8_TYPELESS,
	R8_UNORM,
	R8_UINT,
	R8_SNORM,
	R8_SINT,
	A8_UNORM,
	R1_UNORM,
	R9G9B9E5_SHAREDEXP,
	R8G8_B8G8_UNORM,
	G8R8_G8B8_UNORM,
	BC1_TYPELESS,
	BC1_UNORM,
	BC1_UNORM_SRGB,
	BC2_TYPELESS,
	BC2_UNORM,
	BC2_UNORM_SRGB,
	BC3_TYPELESS,
	BC3_UNORM,
	BC3_UNORM_SRGB,
	BC4_TYPELESS,
	BC4_UNORM,
	BC4_SNORM,
	BC5_TYPELESS,
	BC5_UNORM,
	BC5_SNORM,
	B5G6R5_UNORM,
	B5G5R5A1_UNORM,
	B8G8R8A8_UNORM,
	B8G8R8X8_UNORM,
	R10G10B10_XR_BIAS_A2_UNORM,
	B8G8R8A8_TYPELESS,
	B8G8R8A8_UNORM_SRGB,
	B8G8R8X8_TYPELESS,
	B8G8R8X8_UNORM_SRGB,
	B4G4R4A4_UNORM
};

struct Format
{
	Format(EFormat enumValue = EFormat::UNKNOWN);

	DXGI_FORMAT ToDirectX() const;
	unsigned int ToOpenGL() const;
	VkFormat ToVulkan() const;
	VkIndexType ToVulkanIndexType() const;
	operator EFormat() const;

	EFormat enum_value;
};

enum class ETopology
{
	UNDEFINED,
	POINTLIST,
	LINELIST,
	LINESTRIP,
	TRIANGLELIST,
	TRIANGLESTRIP,
	LINELIST_ADJ,
	LINESTRIP_ADJ,
	TRIANGLELIST_ADJ,
	TRIANGLESTRIP_ADJ,
};

struct Topology
{
	Topology(ETopology enumValue = ETopology::UNDEFINED);

	D3D_PRIMITIVE_TOPOLOGY ToDirectX() const;
	D3D12_PRIMITIVE_TOPOLOGY_TYPE ToDirectX12() const;
	unsigned int ToOpenGL() const;
	VkPrimitiveTopology ToVulkan() const;

	operator ETopology() const;

	ETopology enum_value;
};

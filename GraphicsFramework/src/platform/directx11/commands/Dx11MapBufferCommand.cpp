#include "stdafx.h"

#include "Dx11MapBufferCommand.h"

#include "../Dx11HelperMethods.h"
#include "../Dx11ShaderCBuffer.h"
#include "../Dx11BufferWrapper.h"

Dx11MapBufferCommand::Dx11MapBufferCommand(const ApiBufferWrapper* buffer, D3D11_MAP mapType, D3D11_MAPPED_SUBRESOURCE* mappedSubresource, const UINT mapFlags, const UINT subresource) :
	MapBufferCommand(buffer),
	m_constant_buffer(nullptr),
	m_map_type(mapType),
	m_mapped_subresource(mappedSubresource),
	m_map_flags(mapFlags),
	m_subresource(subresource)
{

}


Dx11MapBufferCommand::Dx11MapBufferCommand(const Dx11ShaderCBuffer* buffer, D3D11_MAP mapType, D3D11_MAPPED_SUBRESOURCE* mappedSubresource, const UINT mapFlags, const UINT subresource) :
	MapBufferCommand(nullptr),
	m_constant_buffer(buffer),
	m_map_type(mapType),
	m_mapped_subresource(mappedSubresource),
	m_map_flags(mapFlags),
	m_subresource(subresource)
{

}

Dx11MapBufferCommand::~Dx11MapBufferCommand()
{

}

void Dx11MapBufferCommand::Execute()
{
	if (m_constant_buffer)
	{
		DXCALL(GetDx11DeviceContext()->Map(m_constant_buffer->buffer.Get(), m_subresource, m_map_type, m_map_flags, m_mapped_subresource));
	}
	else
	{
		DXCALL(GetDx11DeviceContext()->Map(m_buffer->As<Dx11BufferWrapper>()->GetBuffer(), m_subresource, m_map_type, m_map_flags, m_mapped_subresource));
	}
}
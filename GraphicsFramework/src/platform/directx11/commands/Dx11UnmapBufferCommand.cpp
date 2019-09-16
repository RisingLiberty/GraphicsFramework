#include "stdafx.h"

#include "Dx11UnmapBufferCommand.h"

#include "../Dx11HelperMethods.h"
#include "../Dx11ShaderCBuffer.h"
#include "../Dx11BufferWrapper.h"

Dx11UnmapBufferCommand::Dx11UnmapBufferCommand(const ApiBufferWrapper* buffer, const UINT subresource) :
	UnmapBufferCommand(buffer),
	m_constant_buffer(nullptr),
	m_subresource(subresource)
{

}

Dx11UnmapBufferCommand::Dx11UnmapBufferCommand(const Dx11ShaderCBuffer* buffer, const UINT subresource) :
	UnmapBufferCommand(nullptr),
	m_constant_buffer(buffer),
	m_subresource(subresource)
{

}

Dx11UnmapBufferCommand::~Dx11UnmapBufferCommand()
{

}

void Dx11UnmapBufferCommand::Execute()
{
	if (m_constant_buffer)
		GetDx11DeviceContext()->Unmap(m_constant_buffer->buffer.Get(), m_subresource);
	else
		GetDx11DeviceContext()->Unmap(m_buffer->As<Dx11BufferWrapper>()->GetBuffer(), m_subresource);
}
#include "stdafx.h"

#include "Dx11BindIndexBufferCommand.h"

#include "../Dx11HelperMethods.h"
#include "../Dx11IndexBuffer.h"

Dx11BindIndexBufferCommand::Dx11BindIndexBufferCommand(const IndexBuffer* buffer) :
	BindIndexBufferCommand(buffer)
{

}

Dx11BindIndexBufferCommand::~Dx11BindIndexBufferCommand()
{

}

void Dx11BindIndexBufferCommand::Execute()
{
	const unsigned int offset = 0;
	GetDx11DeviceContext()->IASetPrimitiveTopology(m_index_buffer->GetTopology().ToDirectX());
	GetDx11DeviceContext()->IASetIndexBuffer(m_index_buffer->As<Dx11IndexBuffer>()->GetBuffer(), m_index_buffer->GetFormat().ToDirectX(), offset);
}
#include "stdafx.h"

#include "Dx12VertexBuffer.h"

Dx12VertexBuffer::Dx12VertexBuffer(unsigned int size, BufferUsage usage, BufferAccess access, const void* data):
	VertexBuffer(size),
	Dx12BufferWrapper(size, usage, access, data)
{
}

Dx12VertexBuffer::~Dx12VertexBuffer()
{

}

void Dx12VertexBuffer::SetData(const void* data)
{
	this->SetDataInternal(data, m_size);
}

std::unique_ptr<byte> Dx12VertexBuffer::GetData() const
{
	return this->GetDataInternal(m_size);
}

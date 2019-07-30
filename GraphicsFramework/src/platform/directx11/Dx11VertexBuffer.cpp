#include "stdafx.h"

#include "Dx11VertexBuffer.h"
#include "Dx11Context.h"
#include "Dx11HelperMethods.h"
#include "Dx11DownloadBuffer.h"

Dx11VertexBuffer::Dx11VertexBuffer(unsigned int size, BufferUsage usage, void* data):
	VertexBuffer(size),
	Dx11BufferWrapper(size, usage, BufferType::VERTEX, data)
{
}


Dx11VertexBuffer::~Dx11VertexBuffer()
{
}

void Dx11VertexBuffer::SetData(const void* data)
{
	this->SetDataInternal(data, m_size);
}

std::unique_ptr<byte> Dx11VertexBuffer::GetData() const
{
	return this->GetDataInternal(m_size);
}

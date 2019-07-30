#include "stdafx.h"

#include "Dx11IndexBuffer.h"
#include "Dx11Context.h"
#include "Dx11HelperMethods.h"

Dx11IndexBuffer::Dx11IndexBuffer(unsigned int count, Format format, Topology topology, BufferUsage usage, void* data):
	IndexBuffer(count, format, topology),
	Dx11BufferWrapper(m_size, usage, BufferType::INDEX, data)
{
}


Dx11IndexBuffer::~Dx11IndexBuffer()
{
}

std::unique_ptr<byte> Dx11IndexBuffer::GetData() const
{
	return this->GetDataInternal(m_size);
}

void Dx11IndexBuffer::SetData(const void* data)
{
	this->SetDataInternal(data, m_size);
}

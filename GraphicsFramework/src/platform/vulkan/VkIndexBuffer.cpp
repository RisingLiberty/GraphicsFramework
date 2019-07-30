#include "stdafx.h"

#include "VkIndexBuffer.h"
#include "VkHelperMethods.h"

#include "VkContext.h"

VkIndexBuffer::VkIndexBuffer(unsigned int count, Format format, Topology topology, BufferUsage usage, const void* data):
	IndexBuffer(count, format, topology),
	VkBufferWrapper(m_size, usage, data)
{
	if (data)
		this->Bind();
}

VkIndexBuffer::~VkIndexBuffer()
{
}

void VkIndexBuffer::SetData(const void* data)
{
	this->SetDataInternal(data, m_size);
}

std::unique_ptr<byte> VkIndexBuffer::GetData() const
{
	return this->GetDataInternal(m_size);
}
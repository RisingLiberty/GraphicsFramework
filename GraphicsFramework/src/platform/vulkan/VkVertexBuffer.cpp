#include "stdafx.h"

#include "VkVertexBuffer.h"
#include "VkHelperMethods.h"
#include "VkDownloadBuffer.h"

VkVertexBuffer::VkVertexBuffer(unsigned int size, BufferUsage usage, BufferAccess access, const void* data) :
	VertexBuffer(size),
	VkBufferWrapper(size, usage, access, data)
{
}

VkVertexBuffer::~VkVertexBuffer()
{
}

void VkVertexBuffer::SetData(const void* data)
{
	this->SetDataInternal(data, m_size);
}

std::unique_ptr<byte> VkVertexBuffer::GetData() const
{
	return this->GetDataInternal(m_size);
}

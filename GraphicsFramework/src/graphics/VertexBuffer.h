#pragma once

#include "BufferHelpers.h"
#include "Buffer.h"

class DownloadBuffer;

class VertexBuffer : public Buffer
{
public:

	VertexBuffer(unsigned int size);
	virtual ~VertexBuffer();
		
	static VertexBuffer* Create(unsigned int size, BufferUsage usage = EBufferUsage::STATIC, void* data = nullptr);

	bool operator==(const VertexBuffer* other) const;

};
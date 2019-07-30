#pragma once

#include "GraphicsResource.h"

class Buffer : public GraphicsResource<Buffer>
{
public: 
	Buffer(unsigned int size);
	virtual ~Buffer();

	virtual std::unique_ptr<byte> GetData() const = 0;
	virtual void SetData(const void* data) = 0;

	unsigned int GetSize() const;

protected:
	unsigned int m_size;
};
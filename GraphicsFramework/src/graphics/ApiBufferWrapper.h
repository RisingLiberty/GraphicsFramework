#pragma once

#include "BufferHelpers.h"
#include "GraphicsResource.h"

class DownloadBuffer;

class ApiBufferWrapper : public GraphicsResource<ApiBufferWrapper>
{
public:
	ApiBufferWrapper(BufferUsage usage);
	virtual ~ApiBufferWrapper();

protected:
	std::unique_ptr<byte> GetDataInternal(unsigned int size) const;
	virtual void SetDataInternal(const void* data, unsigned int size) = 0;
	virtual std::unique_ptr<DownloadBuffer> DownloadDataToBuffer(unsigned int size) const = 0;

protected:
	BufferUsage m_usage;
};
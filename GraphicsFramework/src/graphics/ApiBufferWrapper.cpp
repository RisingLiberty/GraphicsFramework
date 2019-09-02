#include "stdafx.h"

#include "ApiBufferWrapper.h"
#include "DownloadBuffer.h"

ApiBufferWrapper::ApiBufferWrapper(BufferUsage usage, BufferAccess access):
	m_usage(usage),
	m_access(access)
{
	
}

ApiBufferWrapper::~ApiBufferWrapper()
{

}

std::unique_ptr<byte> ApiBufferWrapper::GetDataInternal(unsigned int size) const
{
	std::unique_ptr<DownloadBuffer> download_buffer = this->DownloadDataToBuffer(size);
	byte* data = (byte*)malloc(size);
	memcpy(data, download_buffer->GetCpuAddress(), size);
	std::unique_ptr<byte> unique_data;
	unique_data.reset(data);
	return std::move(unique_data);
}

BufferUsage ApiBufferWrapper::GetUsage() const
{
	return m_usage;
}

BufferAccess ApiBufferWrapper::GetAccess() const
{
	return m_access;
}
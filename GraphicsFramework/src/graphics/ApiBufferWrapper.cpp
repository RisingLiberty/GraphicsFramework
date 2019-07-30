#include "stdafx.h"

#include "ApiBufferWrapper.h"
#include "DownloadBuffer.h"

ApiBufferWrapper::ApiBufferWrapper(BufferUsage usage):
	m_usage(usage)
{
	
}

ApiBufferWrapper::~ApiBufferWrapper()
{

}

std::unique_ptr<byte> ApiBufferWrapper::GetDataInternal(unsigned int size) const
{
	std::unique_ptr<DownloadBuffer> download_buffer = this->DownloadDataToBuffer(size);
	byte* data = (byte*)malloc(size);
	memcpy(data, download_buffer->GetData(), size);
	std::unique_ptr<byte> unique_data;
	unique_data.reset(data);
	return std::move(unique_data);
}
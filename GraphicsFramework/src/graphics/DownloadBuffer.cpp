#include "stdafx.h"

#include "DownloadBuffer.h"

DownloadBuffer::DownloadBuffer()
{

}

DownloadBuffer::~DownloadBuffer()
{
	delete m_data;
}

void* DownloadBuffer::GetData() const
{
	return m_data;
}
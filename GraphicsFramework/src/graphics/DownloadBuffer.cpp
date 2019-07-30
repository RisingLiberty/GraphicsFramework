#include "stdafx.h"

#include "DownloadBuffer.h"

DownloadBuffer::DownloadBuffer(unsigned int size):
	m_size(size)
{
	m_data = nullptr;
}

DownloadBuffer::~DownloadBuffer()
{
	delete m_data;
}

void* DownloadBuffer::GetData() const
{
	return m_data;
}

unsigned int DownloadBuffer::GetSize() const
{
	return m_size;
}
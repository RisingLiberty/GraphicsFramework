#include "stdafx.h"

#include "DownloadBuffer.h"

DownloadBuffer::DownloadBuffer(unsigned int size):
	m_size(size)
{
	m_cpu_address = malloc(m_size);
}

DownloadBuffer::~DownloadBuffer()
{
	delete m_cpu_address;
}

void* DownloadBuffer::GetCpuAddress() const
{
	return m_cpu_address;
}

unsigned int DownloadBuffer::GetSize() const
{
	return m_size;
}
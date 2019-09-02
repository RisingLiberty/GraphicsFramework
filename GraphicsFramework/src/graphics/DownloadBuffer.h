#pragma once

class ApiBufferWrapper;

class DownloadBuffer
{
public:
	DownloadBuffer(unsigned int size);
	virtual ~DownloadBuffer();

	virtual void Download(const ApiBufferWrapper* buffer) = 0;

	void* GetCpuAddress() const;
	unsigned int GetSize() const;

protected:
	unsigned int m_size;
	void* m_cpu_address;
	void* m_gpu_address;
};
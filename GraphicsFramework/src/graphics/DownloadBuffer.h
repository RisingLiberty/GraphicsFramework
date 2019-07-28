#pragma once

class DownloadBuffer
{
public:
	DownloadBuffer();
	virtual ~DownloadBuffer();

	virtual void Download(unsigned int size) = 0;

	void* GetData() const;

protected:
	void* m_data;
};
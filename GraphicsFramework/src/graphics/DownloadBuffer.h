#pragma once

class VertexBuffer;

class DownloadBuffer
{
public:
	DownloadBuffer();
	virtual ~DownloadBuffer();

	virtual void Download(const VertexBuffer* vb) = 0;

	void* GetData() const;

protected:
	void* m_data;
};
#pragma once

#include "graphics/ApiBufferWrapper.h"

class GLBufferWrapper : public ApiBufferWrapper
{
public:
	GLBufferWrapper(unsigned int size, BufferUsage usage, BufferType type, const void* data);
	virtual ~GLBufferWrapper();

	BufferType GetType() const;
	unsigned int GetId() const;

protected:
	virtual void GLBind() const = 0;
	void SetDataInternal(const void* data, unsigned int size) override;
	std::unique_ptr<DownloadBuffer> DownloadDataToBuffer(unsigned int size) const override;
	
protected:
	unsigned int m_id;

private:
	BufferType m_type;
};
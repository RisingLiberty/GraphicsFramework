#pragma once

#include "graphics/ApiBufferWrapper.h"

class OpenGLBufferWrapper : public ApiBufferWrapper
{
public:
	OpenGLBufferWrapper(unsigned int size, BufferUsage usage, BufferType type, const void* data);
	~OpenGLBufferWrapper();

	BufferType GetType() const;

protected:
	virtual void GLBind() const = 0;
	void SetDataInternal(const void* data, unsigned int size) override;
	std::unique_ptr<DownloadBuffer> DownloadDataToBuffer(unsigned int size) const override;
	
protected:
	unsigned int m_id;

private:
	BufferType m_type;
};
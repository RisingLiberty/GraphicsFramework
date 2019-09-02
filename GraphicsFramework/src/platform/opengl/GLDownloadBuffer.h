#pragma once

#include "graphics/DownloadBuffer.h"
#include "GLBufferWrapper.h"

class GLDirectCommandList;

class GLDownloadBuffer : public GLBufferWrapper, public DownloadBuffer
{
public:
	GLDownloadBuffer(unsigned int size);
	virtual ~GLDownloadBuffer();

	void Download(const ApiBufferWrapper* buffer) override;
	virtual void GLBind() const;

private:
	std::unique_ptr<GLDirectCommandList> m_cmd_list;
};
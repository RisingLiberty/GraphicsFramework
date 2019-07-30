#pragma once

#include "graphics/DownloadBuffer.h"

class OpenGLDownloadBuffer : public DownloadBuffer
{
public:
	OpenGLDownloadBuffer(unsigned int size);
	virtual ~OpenGLDownloadBuffer();

	void Download(const ApiBufferWrapper* buffer) override;

};
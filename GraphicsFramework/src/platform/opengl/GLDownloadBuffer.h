#pragma once

#include "graphics/DownloadBuffer.h"

class GLDownloadBuffer : public DownloadBuffer
{
public:
	GLDownloadBuffer(unsigned int size);
	virtual ~GLDownloadBuffer();

	void Download(const ApiBufferWrapper* buffer) override;

};
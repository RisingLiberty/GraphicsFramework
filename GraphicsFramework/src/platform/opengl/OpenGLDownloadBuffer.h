#pragma once

#include "graphics/DownloadBuffer.h"

class OpenGLDownloadBuffer : public DownloadBuffer
{
public:
	OpenGLDownloadBuffer();
	virtual ~OpenGLDownloadBuffer();

	void Download(const VertexBuffer* vb) override;

};
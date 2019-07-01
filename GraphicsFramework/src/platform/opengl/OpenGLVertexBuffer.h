#pragma once

#include "graphics/VertexBuffer.h"

class OpenGLVertexBuffer : public VertexBuffer
{
	OpenGLVertexBuffer(size_t size);
	virtual ~OpenGLVertexBuffer();
};
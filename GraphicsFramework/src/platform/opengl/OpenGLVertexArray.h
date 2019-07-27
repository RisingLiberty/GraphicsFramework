#pragma once

#include "graphics/VertexArray.h"
#include "graphics/VertexAttribute.h"

class OpenGLVertexArray : public VertexArray
{
public:
	OpenGLVertexArray(const VertexBuffer* vb, const VertexLayout* layout);
	virtual ~OpenGLVertexArray();

	unsigned int GetId() const;

private:
	void EnableAttributes() const;

private:
	unsigned int m_id;
};
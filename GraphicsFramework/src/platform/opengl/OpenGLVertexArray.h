#pragma once

#include <graphics/VertexArray.h>

class OpenGLVertexArray : public VertexArray
{
public:
	OpenGLVertexArray(const VertexBuffer* vb, const VertexLayout* layout);
	virtual ~OpenGLVertexArray();

	virtual void Bind() const override;
	virtual void Unbind() const override;

private:
	unsigned int m_id;
};
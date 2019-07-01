#pragma once

#include <graphics/VertexArray.h>

class OpenGLVertexArray : public VertexArray
{
public:
	OpenGLVertexArray();
	virtual ~OpenGLVertexArray();

	void BindLayoutToBuffer(const VertexBuffer* vb, const VertexLayout* pLayout);

	virtual void Bind() const override;
	virtual void Unbind() const override;

private:
	unsigned int m_id;
};
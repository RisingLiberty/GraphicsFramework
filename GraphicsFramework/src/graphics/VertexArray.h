#pragma once

class VertexBuffer;
class VertexLayout;

class VertexArray
{
public:
	VertexArray();
	virtual ~VertexArray();

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
	virtual void BindLayoutToBuffer(const VertexBuffer* vb, const VertexLayout* pLayout) = 0;

};
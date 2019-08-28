#pragma once

#include "graphics/VertexArray.h"
#include "graphics/VertexAttribute.h"

class GLVertexArray : public VertexArray
{
public:
	GLVertexArray(const VertexBuffer* vb, const VertexLayout* layout);
	virtual ~GLVertexArray();

	unsigned int GetId() const;

private:
	void EnableAttributes() const;

private:
	unsigned int m_id;
};
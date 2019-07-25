#pragma once

#include "graphics/VertexArray.h"

class VkVertexArray : public VertexArray
{
public:
	VkVertexArray(const VertexBuffer* vb, const VertexLayout* layout);
	virtual ~VkVertexArray();

	virtual void Bind() const override;
	virtual void Unbind() const override;
};
#pragma once

#include "graphics/VertexArray.h"

class Dx12VertexArray : public VertexArray
{
public:
	Dx12VertexArray(VertexBuffer* vb, VertexLayout* layout);
	virtual ~Dx12VertexArray();

	void Bind() const override;
	void Unbind() const override;
};
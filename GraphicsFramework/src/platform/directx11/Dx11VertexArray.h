#pragma once

#include "graphics/VertexArray.h"

class Dx11VertexArray : public VertexArray
{
public:
	Dx11VertexArray(const VertexBuffer* vb, const VertexLayout* layout);
	virtual ~Dx11VertexArray();

	virtual void Bind() const override;
	virtual void Unbind() const override;

};


#pragma once

#include "graphics/VertexArray.h"

class Dx12VertexArray : public VertexArray
{
public:
	Dx12VertexArray(const VertexBuffer* vb, const VertexLayout* layout);
	virtual ~Dx12VertexArray();

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const;

	void Bind() const override;
	void Unbind() const override;
};
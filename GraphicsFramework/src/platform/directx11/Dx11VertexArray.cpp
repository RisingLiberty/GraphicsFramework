#include "stdafx.h"

#include "Dx11VertexArray.h"

#include "Dx11Context.h"
#include "Dx11HelperMethods.h"
#include "Dx11VertexBuffer.h"
#include "Dx11VertexLayout.h"
#include "Dx11VertexShader.h"

Dx11VertexArray::Dx11VertexArray(const VertexBuffer* vb, const VertexLayout* layout):
	VertexArray(vb, layout)
{
}


Dx11VertexArray::~Dx11VertexArray()
{
}

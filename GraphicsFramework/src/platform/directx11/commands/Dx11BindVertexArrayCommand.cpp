#include "stdafx.h"

#include "Dx11BindVertexArrayCommand.h"

#include "../Dx11HelperMethods.h"
#include "../Dx11VertexArray.h"
#include "../Dx11VertexBuffer.h"
#include "../Dx11VertexLayout.h"

Dx11BindVertexArrayCommand::Dx11BindVertexArrayCommand(const VertexArray* va) :
	BindVertexArrayCommand(va)
{

}

Dx11BindVertexArrayCommand::~Dx11BindVertexArrayCommand()
{

}

void Dx11BindVertexArrayCommand::Execute()
{
	const unsigned int offset = 0;
	const unsigned int input_slot = 0;

	const Dx11VertexBuffer* dx_vb = m_vertex_array->GetVertexBuffer()->As<Dx11VertexBuffer>();
	const Dx11VertexLayout* dx_layout = m_vertex_array->GetVertexLayout()->As<Dx11VertexLayout>();

	ID3D11Buffer* buffer = dx_vb->GetBuffer();
	UINT stride = dx_layout->GetSize();

	GetDx11DeviceContext()->IASetInputLayout(dx_layout->GetLayout());
	GetDx11DeviceContext()->IASetVertexBuffers(input_slot, 1, &buffer, &stride, &offset);
}
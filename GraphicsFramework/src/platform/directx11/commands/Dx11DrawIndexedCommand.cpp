#include "stdafx.h"

#include "Dx11DrawIndexedCommand.h"

#include "../Dx11HelperMethods.h"

Dx11DrawIndexedCommand::Dx11DrawIndexedCommand(const unsigned int count) :
	DrawIndexedCommand(count)
{

}

Dx11DrawIndexedCommand::~Dx11DrawIndexedCommand()
{

}

void Dx11DrawIndexedCommand::Execute()
{
	const unsigned int index_start_location = 0;
	const unsigned int base_vertex_location = 0;

	GetDx11DeviceContext()->DrawIndexed(m_count, index_start_location, base_vertex_location);
}
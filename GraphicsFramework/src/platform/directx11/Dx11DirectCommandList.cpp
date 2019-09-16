#include "stdafx.h"

#include "Dx11DirectCommandList.h"

Dx11DirectCommandList::Dx11DirectCommandList():
	m_has_executed(true)
{
	this->Open();
}

Dx11DirectCommandList::~Dx11DirectCommandList()
{
	if (!m_has_executed)
	{
		this->Close();
		this->Execute();
	}
}

void Dx11DirectCommandList::Execute()
{
	Dx11CommandList::Execute();
	m_has_executed = true;
}
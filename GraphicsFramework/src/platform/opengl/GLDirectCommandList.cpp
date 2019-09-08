#include "stdafx.h"

#include "GLDirectCommandList.h"

GLDirectCommandList::GLDirectCommandList():
	m_has_executed(false)
{
	this->Open();
}

GLDirectCommandList::~GLDirectCommandList()
{
	this->Close();
	if (!m_has_executed)
		this->Execute();
}

void GLDirectCommandList::Execute()
{
	GLCommandList::Execute();
	m_has_executed = true;
}

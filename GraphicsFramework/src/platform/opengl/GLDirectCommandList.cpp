#include "stdafx.h"

#include "GLDirectCommandList.h"

GLDirectCommandList::GLDirectCommandList()
{
	this->Open();
}

GLDirectCommandList::~GLDirectCommandList()
{
	this->Close();
	this->Execute();
}

#include "stdafx.h"

#include "CopyToPointerCommand.h"

CopyToPointerCommand::CopyToPointerCommand(void* dst, void** src, unsigned int size) :
	m_destination(dst),
	m_source(src),
	m_size(size)
{

}

CopyToPointerCommand::~CopyToPointerCommand()
{

}

void CopyToPointerCommand::Execute()
{
	memcpy(m_destination, *m_source, m_size);
}

std::string CopyToPointerCommand::ToString() const
{
	return "Copy to pointer command";
}
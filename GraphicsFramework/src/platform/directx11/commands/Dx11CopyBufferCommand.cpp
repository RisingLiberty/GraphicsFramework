#include "stdafx.h"

#include "Dx11CopyBufferCommand.h"

#include "../Dx11HelperMethods.h"

#include "../Dx11BufferWrapper.h"

Dx11CopyBufferCommand::Dx11CopyBufferCommand(const unsigned int size, ApiBufferWrapper* dest, const ApiBufferWrapper* src) :
	CopyBufferCommand(size, dest, src)
{

}

Dx11CopyBufferCommand::~Dx11CopyBufferCommand()
{

}

void Dx11CopyBufferCommand::Execute()
{
	GetDx11DeviceContext()->CopyResource(m_destination->As<Dx11BufferWrapper>()->GetBuffer(), m_source->As<Dx11BufferWrapper>()->GetBuffer());
}
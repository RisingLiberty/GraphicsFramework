#include "stdafx.h"

#include "Buffer.h"

Buffer::Buffer(unsigned int size) :
	m_size(size)
{

}

Buffer::~Buffer()
{

}

unsigned int Buffer::GetSize() const
{
	return m_size;
}
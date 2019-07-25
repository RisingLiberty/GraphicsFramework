#include "stdafx.h"

#include "VkVertexArray.h"
#include "VkHelperMethods.h"
#include "VkContext.h"

VkVertexArray::VkVertexArray(const VertexBuffer* vb, const VertexLayout* layout) :
	VertexArray(vb, layout)
{
	GetVkContext()->BindVertexArray(this);
}

VkVertexArray::~VkVertexArray()
{

}

void VkVertexArray::Bind() const
{

}

void VkVertexArray::Unbind() const
{

}
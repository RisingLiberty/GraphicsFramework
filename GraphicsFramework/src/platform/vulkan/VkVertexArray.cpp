#include "stdafx.h"

#include "VkVertexArray.h"
#include "VkHelperMethods.h"
#include "VkContext.h"

VkVertexArray::VkVertexArray(const VertexBuffer* vb, const VertexLayout* layout) :
	VertexArray(vb, layout)
{
	this->Bind();
}

VkVertexArray::~VkVertexArray()
{

}

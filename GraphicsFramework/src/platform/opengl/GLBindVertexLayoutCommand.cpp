#include "stdafx.h"

#include "GLBindVertexLayoutCommand.h"
#include "GLHelperMethods.h"

#include "graphics/VertexLayout.h"
#include "graphics/VertexAttribute.h"

GLBindVertexLayoutCommand::GLBindVertexLayoutCommand(const VertexLayout* vertexLayout) :
	BindVertexLayoutCommand(vertexLayout)
{

}

GLBindVertexLayoutCommand::~GLBindVertexLayoutCommand()
{

}

void GLBindVertexLayoutCommand::Execute()
{
	const std::vector<VertexAttribute>& attributes = m_vertex_layout->GetAttributes();
	size_t offset = 0;

	for (unsigned int i = 0; i < attributes.size(); ++i)
	{
		const VertexAttribute& attribute = attributes[i];
		GLCALL(glEnableVertexAttribArray(i));
		GLCALL(glVertexAttribPointer(i, attribute.count, attribute.GetGLDataType(), attribute.is_normalized, m_vertex_layout->GetSize(), reinterpret_cast<const void*>(offset)));
		offset += attribute.GetSize();
	}
}
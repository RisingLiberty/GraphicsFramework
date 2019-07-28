#include "stdafx.h"

#include "MeshFactory.h"
#include "Mesh.h"

#include "VertexBuffer.h"
#include "VertexLayout.h"
#include "IndexBuffer.h"

std::unique_ptr<Mesh> MeshFactory::CreateQuad()
{
	std::array<float, 12> positions = 
	{
		-0.5f, -0.5f, 0.0f,	//0
		 0.5f, -0.5f, 0.0f,	//1
		 0.5f,  0.5f, 0.0f,	//2
		-0.5f,  0.5f, 0.0f	//3
	};

	std::array<unsigned short, 6> indices = 
	{
		0,1,2,
		2,3,0
	};

	VertexBuffer* vb = VertexBuffer::Create(sizeof(positions) , BufferUsage::STATIC, positions.data());
	IndexBuffer* ib = IndexBuffer::Create((unsigned int )indices.size(), Format(EFormat::R16_UINT), Topology(ETopology::TRIANGLELIST), BufferUsage::STATIC, indices.data());
	VertexLayout* layout = VertexLayout::Create();
	layout->Push<float>(VertexAttributeType::POSITION, 3);

	std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(vb, layout, ib);

	return mesh;
}

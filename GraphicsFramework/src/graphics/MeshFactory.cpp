#include "stdafx.h"

#include "MeshFactory.h"
#include "Mesh.h"

#include "VertexBuffer.h"
#include "VertexLayout.h"
#include "IndexBuffer.h"

std::unique_ptr<Mesh> MeshFactory::CreateQuad()
{
	std::array<float, 8> positions = 
	{
		-0.5f, -0.5f, //0
		 0.5f, -0.5f, //1
		 0.5f,  0.5f, //2
		-0.5f,  0.5f  //3
	};

	std::array<int, 6> indices = 
	{
		0,1,2,
		2,3,0
	};

	std::unique_ptr<VertexBuffer> vb = VertexBuffer::Create(sizeof(positions));
	std::unique_ptr<IndexBuffer> ib = IndexBuffer::Create(6);
	std::unique_ptr<VertexLayout> layout = std::make_unique<VertexLayout>();
	layout->Push<float>(VertexAttributeType::POSITION, 2);

	vb->SetData(positions.data());
	ib->SetData(indices.data());

	std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(vb, layout, ib);
	return mesh;
}

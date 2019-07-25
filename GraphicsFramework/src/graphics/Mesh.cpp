#include "stdafx.h"

#include "Mesh.h"

////Define to include the function bodies and avoid linker issues
//#ifndef TINYOBJLOADER_IMPLEMENTATION
//#define TINYOBJLOADER_IMPLEMENTATION
//#include <objreader/tiny_obj_loader.h>
//#endif

#include "VertexBuffer.h"
#include "VertexLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

Mesh::Mesh(std::unique_ptr<VertexBuffer>& vb, std::unique_ptr<VertexLayout>& layout, std::unique_ptr<IndexBuffer>& ib):
	m_vertices(std::move(vb)),
	m_vertex_layout(std::move(layout)),
	m_indices(std::move(ib))
{
	VertexArray::Create(m_vertices.get(), m_vertex_layout.get());
}

//Mesh::Mesh(const std::string& path)
//{
//	tinyobj::attrib_t attrib;
//	std::vector<tinyobj::shape_t> shapes;
//	std::vector<tinyobj::material_t> materials;
//	std::string warning;
//	std::string err;
//
//	//The attrib containter holds all of the positions, normals and texture coordinates in its attrib.vertices,
//	//attrib.normals and attrib.texcoords vectors.
//	//The shapes container contains all of the separate objects and their faces. each face consists of an array
//	//of vertices, and each vertex contains the indices of the position, normal and texture coordinate attributes.
//	//The warning and err string contain warning and errors that occurred while loading the file, lie a missing material
//	//definition. Loading only really failed if the LoadObj function returns false.
//	//As mentioned above, faces in OBJ files can actually contain an arbitrary number of vertices, whereas our application
//	//can only render triangles. Luckily the LoadObj has an optional parameter to automatically triangulate such faces,
//	//which is enabled by default.
//	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &err, path.c_str()))
//		throw std::runtime_error(err);
//
//	std::vector<Vertex> vertices;
//	std::vector<int> indices;
//	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};
//
//	for (const tinyobj::shape_t& shape : shapes)
//	{
//		for (const tinyobj::index_t& index : shape.mesh.indices)
//		{
//			Vertex vertex = {};
//
//			//For simplicity, we will assume that every vertex is unqie for now, hence the simple auto-increment indices.
//			//The index variable is of type tinyobj::index_t, which contains the vertex_index, normal_index and texcoord_index members.
//			//We need to use these indices to look up the actual vertex attributes in the attrib arrays
//
//			//Unfortunately the attrib.vertices array is an array of float values instead of something like glm::vec3,
//			//so you need to multiply the index by 3.
//			//Similarly there are 2 texture coordinate components per entry. 
//			//The offsets of 0,1 and 2 are used to access the x, y and z components, or the u and v components in the case of texture coordinates
//			vertex.Position =
//			{
//				attrib.vertices[3 * index.vertex_index + 0],
//				attrib.vertices[3 * index.vertex_index + 1],
//				attrib.vertices[3 * index.vertex_index + 2]
//			};
//
//			vertex.TexCoord =
//			{
//				attrib.texcoords[2 * index.texcoord_index + 0],
//
//				//OpenGL expects the top to be 0 while vulkan expect it to be the bottom
//				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
//			};
//
//			vertex.Color = { 1.0f, 1.0f, 1.0f };
//
//			//Every time we read a vertex from the OBJ file, we check if we've already seen a vertex
//			//with the exact same position and textrue coordinates before. if not, we add it to vertices
//			//and store its index in the uniqueVertices container. after that we add the index of the new vertex
//			//to indices. if We've seen the exact same vertex before, then we look up its index in uniqueVertices and store
//			//that index in m_Indices.
//
//			if (uniqueVertices.count(vertex) == 0)
//			{
//				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
//				vertices.push_back(vertex);
//			}
//
//			indices.push_back(uniqueVertices[vertex]);
//		}
//	}
//
//	m_vertices = VertexBuffer::Create(vertices.size() * sizeof(Vertex), BufferUsage::STATIC);
//	m_indices = IndexBuffer::Create(indices.size());
//}

Mesh::~Mesh()
{

}

VertexBuffer* Mesh::GetVertices() const
{
	return m_vertices.get();
}

VertexLayout* Mesh::GetVertexLayout() const
{
	return m_vertex_layout.get();
}

IndexBuffer* Mesh::GetIndices() const
{
	return m_indices.get();
}
#include "stdafx.h"

#include "SceneObject.h"

#include "graphics/Mesh.h"
#include "graphics/Material.h"

SceneObject::SceneObject(std::unique_ptr<Mesh>& mesh, std::unique_ptr<Material>& material):
	m_mesh(std::move(mesh)),
	m_material(std::move(material))
{

}

SceneObject::~SceneObject()
{

}

Mesh* SceneObject::GetMesh() const
{
	return m_mesh.get();
}

Material* SceneObject::GetMaterial() const
{
	return m_material.get();
}
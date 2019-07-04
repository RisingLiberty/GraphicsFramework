#include "stdafx.h"

#include "SceneObject.h"

#include "graphics/Mesh.h"
#include "graphics/Material.h"
#include "graphics/Context.h"
#include "graphics/Renderer.h"


SceneObject::SceneObject(const std::string& name, std::unique_ptr<Mesh>& mesh, std::unique_ptr<Material>& material):
	LayerObject(name),
	m_mesh(std::move(mesh)),
	m_material(std::move(material))
{

}

SceneObject::~SceneObject()
{

}

void SceneObject::Update(float dTime)
{
	UNREFERENCED_PARAMETER(dTime);
}

void SceneObject::Draw()
{
	Context::GetCurrent()->GetRenderer()->SubmitObject(this);
}

Mesh* SceneObject::GetMesh() const
{
	return m_mesh.get();
}

Material* SceneObject::GetMaterial() const
{
	return m_material.get();
}
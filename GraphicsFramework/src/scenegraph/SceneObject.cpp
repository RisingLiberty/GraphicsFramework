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
	static float direction = 1.0f;
	static float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	color[1] += dTime * direction;

	if (color[1] >= 1.0f)
	{
		color[1] = 1.0f;
		direction *= -1;
	}
	else if (color[1] <= 0.0f)
	{
		color[1] = 0.0f;
		direction *= -1;
	}

	m_material->SetParameter("u_Color", color);
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
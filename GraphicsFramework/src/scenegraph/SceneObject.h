#pragma once

#include "LayerObject.h"

class Mesh;
class Material;

class SceneObject : public LayerObject
{
public:
	SceneObject(const std::string& name, std::unique_ptr<Mesh>& mesh, std::unique_ptr<Material>& material);
	~SceneObject();

	void Update(float dTime);
	void Draw();

	Mesh* GetMesh() const;
	Material* GetMaterial() const;

private:
	std::unique_ptr<Mesh> m_mesh;
	std::unique_ptr<Material> m_material;
};
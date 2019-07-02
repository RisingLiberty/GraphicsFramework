#pragma once

class Mesh;
class Material;

class SceneObject
{
public:
	SceneObject(std::unique_ptr<Mesh>& mesh, std::unique_ptr<Material>& material);
	~SceneObject();

	Mesh* GetMesh() const;
	Material* GetMaterial() const;

private:
	std::unique_ptr<Mesh> m_mesh;
	std::unique_ptr<Material> m_material;
};
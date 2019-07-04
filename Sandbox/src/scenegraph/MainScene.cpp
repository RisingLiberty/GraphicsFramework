#include "pch.h"

#include "MainScene.h"

#include <controllers/ShaderController.h>

#include <graphics/Context.h>
#include <graphics/MeshFactory.h>
#include <graphics/Mesh.h>
#include <graphics/Renderer.h>
#include <graphics/Material.h>
#include <graphics/VertexShader.h>
#include <graphics/FragmentShader.h>
#include <graphics/ShaderProgram.h>


#include <scenegraph/SceneObject.h>

MainScene::MainScene(const std::string& name) :
	Scene(name)
{

	VertexShader* vs = VertexShader::Create("vertexshader");
	FragmentShader* fs = FragmentShader::Create("fragmentshader");

	ShaderProgram* program = ShaderProgram::Create(vs, fs);
	std::unique_ptr<Material> material = std::make_unique<Material>(program);
	float color[] = { 1.0f, 0.3f, 0.8f, 1.0f };
	material->SetParameter("u_Color", color);
	
	std::unique_ptr<Mesh> mesh = MeshFactory::CreateQuad();

	m_test_quad = std::make_unique<SceneObject>(mesh, material);
}

MainScene::~MainScene() = default;

void MainScene::Update(float dTime)
{
	Scene::Update(dTime);
}

void MainScene::Draw()
{
	Scene::Draw();
	Context::GetCurrent()->GetRenderer()->SubmitMesh(m_test_quad.get());

}
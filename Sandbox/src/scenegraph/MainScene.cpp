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
	std::unique_ptr<Mesh> mesh = MeshFactory::CreateQuad();

	VertexShader* vs = VertexShader::Create("data/shaders/opengl/vertexshader.glsl");
	FragmentShader* fs = FragmentShader::Create("data/shaders/opengl/fragmentshader.glsl");

	ShaderProgram* program = ShaderProgram::Create(vs, fs);
	std::unique_ptr<Material> material = std::make_unique<Material>(program);

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
	Context::GetCurrent()->GetRenderer()->Present();

}
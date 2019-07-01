#include "pch.h"

#include "MainScene.h"


#include <graphics/Context.h>
#include <graphics/Mesh.h>
#include <graphics/Renderer.h>

MainScene::MainScene(const std::string& name) :
	Scene(name)
{
	//m_material_ball = std::make_unique<Mesh>("data/meshes/materialball.obj");
}

MainScene::~MainScene() = default;

void MainScene::Update(float dTime)
{
	Scene::Update(dTime);
}

void MainScene::Draw()
{
	Scene::Draw();
	//Context::GetCurrent()->GetRenderer()->SubmitMesh(m_material_ball.get());
	//Context::GetCurrent()->GetRenderer()->Present();

}
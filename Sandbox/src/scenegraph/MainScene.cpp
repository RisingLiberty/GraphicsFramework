#include "pch.h"

#include "MainScene.h"
#include "MainLayer.h"

MainScene::MainScene(const std::string& name) :
	Scene(name)
{
	this->PushLayer(std::make_unique<MainLayer>());
}

MainScene::~MainScene() = default;

void MainScene::Update(float dTime)
{
	Scene::Update(dTime);
}

void MainScene::Draw()
{
	Scene::Draw();
}
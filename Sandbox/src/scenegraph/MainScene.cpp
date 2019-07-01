#include "pch.h"

#include "MainScene.h"

MainScene::MainScene(const std::string& name) :
	Scene(name)
{
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
#include "pch.h"

#include "MainScene.h"
#include "MainLayer.h"
#include "scenegraph/UILayer.h"
#include "scenegraph/UIButton.h"

MainScene::MainScene(const std::string& name) :
	Scene(name)
{
	this->PushLayer(std::make_unique<MainLayer>());

	std::unique_ptr<Layer> ui = UILayer::Create("--UI--");
	ui->PushObject(std::make_unique<UIButton>("Button", 100, 100));
	this->PushLayer(std::move(ui));
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
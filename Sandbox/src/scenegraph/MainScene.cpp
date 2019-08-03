#include "pch.h"

#include "MainScene.h"
#include "MainLayer.h"
#include "scenegraph/UILayer.h"
#include "scenegraph/UIButton.h"

#include <graphics/API.h>
#include <core/Application.h>
#include <events/SwitchApiEvent.h>

MainScene::MainScene(const std::string& name) :
	Scene(name)
{
	this->PushLayer(std::make_unique<MainLayer>());

	std::unique_ptr<Layer> ui = UILayer::Create("--UI--");
	auto switchApi = []()
	{
		Application::GetInstance()->OnEvent(SwitchApiEvent(API::DIRECTX12));
	};

	ui->PushObject(std::make_unique<UIButton>("Switch API", 100, 100, switchApi));
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
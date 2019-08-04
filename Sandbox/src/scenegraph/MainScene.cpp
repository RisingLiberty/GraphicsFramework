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
	auto switchToDirectX11 = []()
	{
		Application::GetInstance()->OnEvent(SwitchApiEvent(API::DIRECTX11));
	};

	auto switchToDirectX12 = []()
	{
		Application::GetInstance()->OnEvent(SwitchApiEvent(API::DIRECTX12));
	};

	auto switchToOpenGL = []()
	{
		Application::GetInstance()->OnEvent(SwitchApiEvent(API::OPENGL));
	};

	auto switchToVulkan = []()
	{
		Application::GetInstance()->OnEvent(SwitchApiEvent(API::VULKAN));
	};


	ui->PushObject(std::make_unique<UIButton>("Switch DirectX 11", 150, 20, switchToDirectX11));
	ui->PushObject(std::make_unique<UIButton>("Switch DirectX 12", 150, 20, switchToDirectX12));
	ui->PushObject(std::make_unique<UIButton>("Switch OpenGL", 150, 20, switchToOpenGL));
	ui->PushObject(std::make_unique<UIButton>("Switch Vulkan", 150, 20, switchToVulkan));
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
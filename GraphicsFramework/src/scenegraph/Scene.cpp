#include "stdafx.h"

#include "Scene.h"

#include "controllers/LayerController.h"
#include "scenegraph/Layer.h"

Scene::Scene(const std::string& name):
	m_name(name)
{
	m_layer_controller = std::make_unique<LayerController>();
}

Scene::~Scene() = default;

void Scene::PushLayer(std::unique_ptr<Layer> layer)
{
	m_layer_controller->Push(layer);
}

void Scene::Update(float dTime)
{
	m_layer_controller->Update(dTime);
}

void Scene::Draw()
{
	m_layer_controller->Draw();
}

const std::string& Scene::GetName() const
{
	return m_name;
}
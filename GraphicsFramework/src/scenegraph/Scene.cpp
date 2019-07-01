#include "stdafx.h"

#include "Scene.h"

Scene::Scene() = default;

Scene::~Scene() = default;

void Scene::Update(float dTime)
{

}

void Scene::Draw()
{

}

const std::string& Scene::GetName() const
{
	return m_name;
}
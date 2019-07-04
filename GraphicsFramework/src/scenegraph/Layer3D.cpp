#include "stdafx.h"

#include "Layer3D.h"

#include "LayerObject.h"

Layer3D::Layer3D(const std::string& name, bool isVisible, bool shouldUpdateWhenInvisible) :
	Layer(name, isVisible, shouldUpdateWhenInvisible)
{

}

Layer3D::~Layer3D()
{

}

void Layer3D::Update(float dTime)
{
	if (!m_should_update_when_invisible && !IsVisible())
	{
		spdlog::warn("Trying to update disbled layer!");
		return;
	}

	for (std::unique_ptr<LayerObject>& object : m_objects)
		object->Update(dTime);
}

void Layer3D::Draw()
{
	if (!IsVisible())
	{
		spdlog::warn("Trying to draw a layer that's not visible!");
		return;
	}

	for (std::unique_ptr<LayerObject>& object : m_objects)
		object->Draw();
}
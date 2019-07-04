#include "stdafx.h"

#include "Layer.h"
#include "LayerObject.h"

Layer::Layer(const std::string& name, bool isVisible, bool shouldUpdateWhenInvisible) :
	m_name(name),
	m_is_visible(isVisible),
	m_should_update_when_invisible(shouldUpdateWhenInvisible)
{

}
Layer::~Layer() = default;

LayerObject* Layer::PushObject(std::unique_ptr<LayerObject>& object)
{
	m_objects.emplace_back(std::move(object));
	return m_objects.back().get();
}

void Layer::IsVisible(bool newIsVisible)
{
	m_is_visible = newIsVisible;
}

const std::string& Layer::GetName() const
{
	return m_name;
}

bool Layer::IsVisible() const
{
	return m_is_visible;
}
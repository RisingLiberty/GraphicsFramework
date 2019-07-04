#include "stdafx.h"

#include "Layer.h"

Layer::Layer(const std::string& name, bool isVisible) :
	m_name(name),
	m_is_visible(isVisible)
{

}
Layer::~Layer() = default;

void Layer::Update(float dTime)
{

}

void Layer::Draw()
{
	if (!m_is_visible)
	{
		spdlog::warn("Trying to draw layer that's not visible! <{}>", m_name);
		return;
	}


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
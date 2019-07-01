#include "stdafx.h"

#include "Layer.h"

Layer::Layer(const std::string & name) :
	m_name(name)
{

}
Layer::~Layer() = default;

void Layer::Update(float dTime)
{

}

void Layer::Draw()
{

}

const std::string& Layer::GetName() const
{
	return m_name;
}
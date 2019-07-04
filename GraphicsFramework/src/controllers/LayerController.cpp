#include "stdafx.h"

#include "LayerController.h"

#include "scenegraph/Layer.h"

LayerController::LayerController() = default;
LayerController::~LayerController() = default;

void LayerController::Push(std::unique_ptr<Layer>& layer)
{
	spdlog::info("{} added", layer->GetName());
	m_layers.push_back(std::move(layer));
}

void LayerController::Update(float dTime)
{
	for (std::unique_ptr<Layer>& layer : m_layers)
	{
		layer->Update(dTime);
	}
}

void LayerController::Draw()
{
	for (std::unique_ptr<Layer>& layer : m_layers)
	{
		layer->Draw();
	}
}
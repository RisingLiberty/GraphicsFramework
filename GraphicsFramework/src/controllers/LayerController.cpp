#include "stdafx.h"

#include "LayerController.h"

#include "scenegraph/Layer.h"

LayerController::LayerController() = default;
LayerController::~LayerController() = default;

void LayerController::Push(std::unique_ptr<Layer> layer)
{
	m_layers.push_back(std::move(layer));
}
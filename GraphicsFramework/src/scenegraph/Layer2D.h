#pragma once

#include "Layer.h"

class Layer2D : public Layer
{
public:
	Layer2D(const std::string& name, bool isVisible = true, bool shouldUpdateWhenInvisible = false);
	virtual ~Layer2D();

	void Update(float dTime) override;
	void Draw() override final;

};
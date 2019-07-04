#pragma once

#include "Layer.h"

class Layer3D : public Layer
{
public:
	Layer3D(const std::string& name, bool isVisible = true, bool shouldUpdateWhenInvisible = false);
	virtual ~Layer3D();

	void Update(float dTime) override;
	void Draw() override final;
};
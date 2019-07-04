#pragma once

#include "Layer2D.h"

class UILayer : public Layer2D
{
public:
	UILayer(const std::string& title);
	virtual ~UILayer();

	static std::unique_ptr<Layer> Create(const std::string& title);

	void Update(float dTime) override;
	void Draw() override;
};
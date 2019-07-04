#pragma once

#include "scenegraph/UILayer.h"

class Dx11UILayer : public UILayer
{
public:
	Dx11UILayer(const std::string& title);
	virtual ~Dx11UILayer();

	void Update(float dTime) override;
	void Draw() override;
};
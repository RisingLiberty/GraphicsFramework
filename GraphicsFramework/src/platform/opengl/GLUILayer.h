#pragma once

#include "scenegraph/UILayer.h"

class GLUILayer : public UILayer
{
public:
	GLUILayer(const std::string& title);
	virtual ~GLUILayer();

	void Update(float dTime) override;
	void Draw() override;
};
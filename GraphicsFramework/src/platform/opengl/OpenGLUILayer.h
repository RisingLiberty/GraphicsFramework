#pragma once

#include "scenegraph/UILayer.h"

class OpenGLUILayer : public UILayer
{
public:
	OpenGLUILayer(const std::string& title);
	virtual ~OpenGLUILayer();

	void Update(float dTime) override;
	void Draw() override;
};
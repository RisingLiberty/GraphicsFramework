#pragma once

#include "scenegraph/UILayer.h"

class VkUILayer : public UILayer
{
public:
	VkUILayer(const std::string& title);
	virtual ~VkUILayer();

	void Update(float dTime) override;
	void Draw() override;

};
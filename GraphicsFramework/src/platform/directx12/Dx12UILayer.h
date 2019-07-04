#pragma once

#include "scenegraph/UILayer.h"

class Dx12UILayer : public UILayer
{
public:
	Dx12UILayer(const std::string& title);
	~Dx12UILayer();
};
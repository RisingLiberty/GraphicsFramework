#pragma once

#include "scenegraph/Layer3D.h"

class SceneObject;

class MainLayer : public Layer3D
{
public:
	MainLayer();
	~MainLayer();

	void Update(float dTime) override;

private:
	LayerObject* m_test_quad;
};
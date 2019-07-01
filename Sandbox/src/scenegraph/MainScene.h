#pragma once

#include <scenegraph/Scene.h>

class MainScene : public Scene
{
public: 
	MainScene(const std::string& name);
	virtual ~MainScene();
};
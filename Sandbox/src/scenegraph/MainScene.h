#pragma once

#include <scenegraph/Scene.h>

class SceneObject;

class MainScene : public Scene
{
public: 
	MainScene(const std::string& name);
	virtual ~MainScene();

	void Update(float dTime) override;
	void Draw() override;

private:
	std::unique_ptr<SceneObject> m_test_quad;
};

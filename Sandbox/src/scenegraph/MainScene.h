#pragma once

#include <scenegraph/Scene.h>

class Mesh;

class MainScene : public Scene
{
public: 
	MainScene(const std::string& name);
	virtual ~MainScene();

	void Update(float dTime) override;
	void Draw() override;

private:
	std::unique_ptr<Mesh> m_material_ball;
};
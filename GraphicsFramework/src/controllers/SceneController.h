#pragma once

class Scene;

class SceneController
{
public:
	SceneController();
	~SceneController();

	void Push(std::unique_ptr<Scene> scene);

	void Update(float dTime);
	void Draw();

	void Clear();

private:
	std::vector<std::unique_ptr<Scene>> m_scenes;
	Scene* m_active_scene;
};
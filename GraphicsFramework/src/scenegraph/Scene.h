#pragma once

class LayerController;

class Scene
{
public: 
	Scene(const std::string& name);
	~Scene();

	void Update(float dTime);
	void Draw();

	const std::string& GetName() const;

private:
	
	std::string m_name;


	std::unique_ptr<LayerController> m_layer_controller;
};
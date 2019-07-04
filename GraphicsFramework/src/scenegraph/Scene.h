#pragma once

class LayerController;
class Layer;

class Scene
{
public: 
	Scene(const std::string& name);
	virtual ~Scene();

	void PushLayer(std::unique_ptr<Layer> layer);

	virtual void Update(float dTime);
	virtual void Draw();

	const std::string& GetName() const;

private:
	
	std::string m_name;

	std::unique_ptr<LayerController> m_layer_controller;
};
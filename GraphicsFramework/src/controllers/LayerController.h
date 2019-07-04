#pragma once

class Layer;

class LayerController
{
public: 
	LayerController();
	~LayerController();

	void Push(std::unique_ptr<Layer>& layer);
	void Update(float dTime);
	void Draw();

private:
	std::vector<std::unique_ptr<Layer>> m_layers;
};
#pragma once

class LayerObject;

class Layer
{
public:
	Layer(const std::string& name, bool isVisible, bool shouldUpdateWhenInvisible);
	virtual ~Layer();

	LayerObject* PushObject(std::unique_ptr<LayerObject>& object);

	virtual void Update(float dTime) = 0;
	virtual void Draw() = 0;

	void IsVisible(bool newIsVisible);

	const std::string& GetName() const;
	bool IsVisible() const;

protected:
	std::vector<std::unique_ptr<LayerObject>> m_objects;
	std::string m_name;
	bool m_is_visible;
	bool m_should_update_when_invisible;
};
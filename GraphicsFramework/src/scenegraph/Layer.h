#pragma once

class Layer
{
public:
	Layer(const std::string& name, bool isVisible = true);
	~Layer();

	void Update(float dTime);
	void Draw();

	void IsVisible(bool newIsVisible);

	const std::string& GetName() const;
	bool IsVisible() const;

private:
	std::string m_name;
	bool m_is_visible;
};
#pragma once

class Layer
{
public:
	Layer(const std::string& name);
	~Layer();

	void Update(float dTime);
	void Draw();

	const std::string& GetName() const;

private:
	std::string m_name;
};
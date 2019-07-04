#pragma once

class LayerObject
{
public:
	LayerObject(const std::string& name);
	virtual ~LayerObject();

	virtual void Update(float dTime) = 0;
	virtual void Draw() = 0;

protected:
	std::string m_name;
};
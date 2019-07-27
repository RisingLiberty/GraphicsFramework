#pragma once

class VertexLayout;

class VertexLayoutController
{
public:
	VertexLayoutController();
	~VertexLayoutController();

	VertexLayout* Push(std::unique_ptr<VertexLayout>& layout);

private:
	std::vector<std::unique_ptr<VertexLayout>> m_layouts;
};
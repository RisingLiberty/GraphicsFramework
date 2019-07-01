#pragma once

class Mesh;

class MeshFactory
{
public:
	static std::unique_ptr<Mesh> CreateQuad();
};
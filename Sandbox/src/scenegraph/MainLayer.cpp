#include "pch.h"

#include "MainLayer.h"

#include <graphics/MeshFactory.h>
#include <graphics/Mesh.h>
#include <graphics/Renderer.h>
#include <graphics/Material.h>
#include <graphics/VertexShader.h>
#include <graphics/FragmentShader.h>
#include <graphics/ShaderProgram.h>

#include "scenegraph/SceneObject.h"

MainLayer::MainLayer():
	Layer3D("Main Layer")
{
	VertexShader* vs = VertexShader::Create("vertexshader");
	FragmentShader* fs = FragmentShader::Create("fragmentshader");

	ShaderProgram* program = ShaderProgram::Create(vs, fs);
	std::unique_ptr<Material> material = std::make_unique<Material>(program);
	float color[] = { 1.0f, 0.3f, 0.8f, 1.0f };
	material->SetParameter("u_Color", color);

	std::unique_ptr<Mesh> mesh = MeshFactory::CreateQuad();
	std::unique_ptr<LayerObject> quad = std::make_unique<SceneObject>("Quad", mesh, material);
	m_test_quad = this->PushObject(std::move(quad));
}

MainLayer::~MainLayer()
{
}

void MainLayer::Update(float dTime)
{

}
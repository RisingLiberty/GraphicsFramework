#pragma once

class Mesh;

class Renderer
{
public: 
	Renderer();
	virtual ~Renderer();

	void SubmitMesh(Mesh* mesh);
	
	virtual void Present() = 0;
	virtual void ClearAllBuffers() = 0;
	virtual void ClearColorBuffer() = 0;
	virtual void ClearDepthStencilBuffer() = 0;
	virtual void ClearDepthBuffer() = 0;
	virtual void ClearStencilBuffer() = 0;


protected:
	std::vector<Mesh*> m_meshes_to_draw;

private:

};
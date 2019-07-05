#pragma once

class SceneObject;

class Renderer
{
public: 
	Renderer();
	virtual ~Renderer();

	void SubmitObject(SceneObject* mesh);
	
	virtual void Present() = 0;
	virtual void ClearAllBuffers() = 0;
	virtual void ClearColorBuffer() = 0;
	virtual void ClearDepthStencilBuffer() = 0;
	virtual void ClearDepthBuffer() = 0;
	virtual void ClearStencilBuffer() = 0;

	virtual void Begin();
	virtual void End();

protected:
	std::vector<SceneObject*> m_scene_objects;
	std::array<float, 4> m_clear_color;

private:

};
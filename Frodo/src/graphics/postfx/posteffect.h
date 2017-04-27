#pragma once

#include <fd.h>
#include <graphics/shader/shader.h>
#include <graphics/texture/texture.h>
#include <graphics/texture/framebuffer2d.h>
#include <graphics/render/mesh/mesh.h>
#include <graphics/render/mesh/meshfactory.h>

namespace FD {

class FDAPI PostEffect {
protected:
	Shader* shader;

	Mesh* quad;
	Texture* texture;

private:
	Framebuffer2D* framebuffer;

public:
	PostEffect(Shader* shader, Texture* texture);
	virtual ~PostEffect();

	void SetPData(const void* data);
	void SetVData(const void* data);

	Framebuffer2D* Render();
	void RenderAndDisplay();

	inline Framebuffer2D* GetFramebuffer() const { return framebuffer; }

	inline Texture* GetTexture() const { return texture; }
	inline void SetTexture(Texture* texture) { this->texture = texture; }
};

}


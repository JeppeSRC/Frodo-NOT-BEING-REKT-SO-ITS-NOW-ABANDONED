#pragma once

#include "framebuffer.h"

namespace FD {

class FDAPI ShadowMapCube : public Framebuffer {
public:
	ShadowMapCube(uint32 w, uint32 h);
	~ShadowMapCube();

	void BindAsRenderTarget() override;
};

}

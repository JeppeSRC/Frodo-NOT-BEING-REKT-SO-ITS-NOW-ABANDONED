#pragma once


#include "framebuffer.h"

namespace FD {

class FDAPI FramebufferCube : public Framebuffer {
public:
	FramebufferCube(uint32 w, uint32 h, FD_TEXTURE_FORMAT format, bool createDepth = true);

	void BindAsRenderTarget() override;
};

}
#pragma once

#include <fd.h>
#include "shader.h"

class FDAPI ShaderFactory {
public:

	static Shader* GetDeferredRenderShader();
	static Shader* GetDeferredCompositionShader();
};
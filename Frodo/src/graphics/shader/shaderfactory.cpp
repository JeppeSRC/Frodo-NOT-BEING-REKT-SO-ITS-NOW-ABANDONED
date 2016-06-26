#include "shaderfactory.h"


static const char* deferred_renderpass_v =
#include "shaders/deferred_renderpass_v.hlsl"
;

static const char* deferred_renderpass_p =
#include "shaders/deferred_renderpass_p.hlsl"
;

static const char* deferred_compositionpass_v =
#include "shaders/deferred_compositionpass_v.hlsl"
;

static const char* deferred_compositionpass_p =
#include "shaders/deferred_compositionpass_p.hlsl"
;


Shader* ShaderFactory::GetDeferredRenderShader() {
	return new Shader(deferred_renderpass_v, deferred_renderpass_p, true);
}

Shader* ShaderFactory::GetDeferredCompositionShader() {
	return new Shader(deferred_compositionpass_v, deferred_compositionpass_p, true);
}
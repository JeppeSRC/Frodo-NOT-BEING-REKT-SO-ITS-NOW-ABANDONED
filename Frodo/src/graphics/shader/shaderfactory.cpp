#include <core/log.h>
#include "shaderfactory.h"

static const char* deferred_geometrypass_v =
#include "shaders/deferred_geometrypass_v.hlsl"
;

static const char* deferred_geometrypass_p =
#include "shaders/deferred_geometrypass_p.hlsl"
;

static const char* deferred_lightingpass_v =
#include "shaders/deferred_lightingpass_v.hlsl"
;

static const char* deferred_directional_light_p =
#include "shaders/deferred_directional_light_p.hlsl"
;

static const char* deferred_point_light_p =
#include "shaders/deferred_point_light_p.hlsl"
;


Shader* ShaderFactory::GetShader(FD_DEFERRED_SHADER_TYPE shader) {
	switch (shader) {
		case FD_DEFERRED_SHADER_TYPE_GEOMETRY:
			return new Shader(deferred_geometrypass_v, deferred_geometrypass_p, true);
		case FD_DEFERRED_SHADER_TYPE_DIRECTIONAL_LIGHT:
			return new Shader(deferred_lightingpass_v, deferred_directional_light_p, true);
		case FD_DEFERRED_SHADER_TYPE_POINT_LIGHT:
			return new Shader(deferred_lightingpass_v, deferred_point_light_p, true);
	}

	FD_WARNING("FD_DEFERRED_SHADER_TYPE_UNKNOWN: Unknown shader returning nullptr");

	return nullptr;
}

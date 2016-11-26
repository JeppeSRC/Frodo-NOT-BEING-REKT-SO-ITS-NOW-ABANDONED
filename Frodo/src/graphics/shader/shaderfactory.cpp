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

static const char* deferred_spot_light_p =
#include "shaders/deferred_spot_light_p.hlsl"
;

static const char* forward_lightingpass_v =
#include "shaders/forward_lightingpass_v.hlsl"
;

static const char* forward_directional_light_p = 
#include "shaders/forward_directional_light_p.hlsl"
;

static const char* forward_point_light_p =
#include "shaders/forward_point_light_p.hlsl"
;

static const char* forward_spot_light_p =
#include "shaders/forward_spot_light_p.hlsl"
;

static const char* font_default_v =
#include "shaders/font_default_v.hlsl"
;

static const char* font_default_p =
#include "shaders/font_default_p.hlsl"
;

static const char* ui_default_v =
#include "shaders/ui_default_v.hlsl"
;

static const char* ui_default_p =
#include "shaders/ui_default_p.hlsl"
;

Shader* ShaderFactory::GetShader(FD_SHADERFACTORY_SHADER_TYPE shader) {
	switch (shader) {
		case FD_DEFERRED_SHADER_GEOMETRY:
			return new Shader(deferred_geometrypass_v, deferred_geometrypass_p, true);
		case FD_DEFERRED_SHADER_DIRECTIONAL_LIGHT:
			return new Shader(deferred_lightingpass_v, deferred_directional_light_p, true);
		case FD_DEFERRED_SHADER_POINT_LIGHT:
			return new Shader(deferred_lightingpass_v, deferred_point_light_p, true);
		case FD_DEFERRED_SHADER_SPOT_LIGHT:
			return new Shader(deferred_lightingpass_v, deferred_spot_light_p, true);
		case FD_FORWARD_SHADER_DIRECTIONAL_LIGHT: 
			return new Shader(forward_lightingpass_v, forward_directional_light_p, true);
		case FD_FORWARD_SHADER_POINT_LIGHT:
			return new Shader(forward_lightingpass_v, forward_point_light_p, true);
		case FD_FORWARD_SHADER_SPOT_LIGHT:
			return new Shader(forward_lightingpass_v, forward_spot_light_p, true);
		case FD_FONT_DEFAULT:
			return new Shader(font_default_v, font_default_p, true);
		case FD_UI_DEFAULT:
			return new Shader(ui_default_v, ui_default_p, true);
	}

	FD_WARNING("FD_SHADER_TYPE_UNKNOWN: Unknown shader returning nullptr");

	return nullptr;
}

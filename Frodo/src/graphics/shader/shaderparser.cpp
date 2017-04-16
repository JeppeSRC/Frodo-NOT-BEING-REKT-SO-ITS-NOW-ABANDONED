#include "shader.h"
#include <math/math.h>
#include <core/log.h>

namespace FD {

enum FD_SHADER_FIELD_TYPE {
	FD_SHADER_FIELD_TYPE_UNKOWN,
	FD_SHADER_FIELD_TYPE_MAT4,
	FD_SHADER_FIELD_TYPE_MAT3,
	FD_SHADER_FIELD_TYPE_VEC4,
	FD_SHADER_FIELD_TYPE_VEC3,
	FD_SHADER_FIELD_TYPE_VEC2,
	FD_SHADER_FIELD_TYPE_FLOAT
};


inline static String get_field_type_as_string(FD_SHADER_FIELD_TYPE type) {
	switch (type) {
		case FD_SHADER_FIELD_TYPE_UNKOWN: return ("UNKOWN");
		case FD_SHADER_FIELD_TYPE_MAT4: return ("float4x4");
		case FD_SHADER_FIELD_TYPE_MAT3: return ("float3x3");
		case FD_SHADER_FIELD_TYPE_VEC4: return ("float4");
		case FD_SHADER_FIELD_TYPE_VEC3: return ("float3");
		case FD_SHADER_FIELD_TYPE_VEC2: return ("float2");
		case FD_SHADER_FIELD_TYPE_FLOAT: return ("float32");
	}
	return ("ERROR");
}

inline static uint32 get_field_type_size(FD_SHADER_FIELD_TYPE type) {
	switch (type) {
		case FD_SHADER_FIELD_TYPE_UNKOWN: return 0;
		case FD_SHADER_FIELD_TYPE_MAT4: return sizeof(mat4);
		case FD_SHADER_FIELD_TYPE_MAT3: return sizeof(mat3);
		case FD_SHADER_FIELD_TYPE_VEC4: return sizeof(vec4);
		case FD_SHADER_FIELD_TYPE_VEC3: return sizeof(vec3);
		case FD_SHADER_FIELD_TYPE_VEC2: return sizeof(vec2);
		case FD_SHADER_FIELD_TYPE_FLOAT: return sizeof(float32);
	}

	return 0;
}

void Shader::RemoveComments(String& source) {

	while (true) {
		uint_t start = source.Find("/*");

		if (start == -1) break;

		uint_t end = source.Find("*/", start);

		source.Remove(start, end + 2);
	}

	while (true) {
		uint_t start = source.Find("//");

		if (start == -1) break;

		uint_t end = source.Find("\n", start);
		if (end == -1) end = source.length;

		source.Remove(start, end);
	}

}

void Shader::ParseStructs(String source, FD_SHADER_TYPE type) {

	while (true) {
		uint_t cbufferStart = source.Find("cbuffer") + 7;

		if (cbufferStart < 7) break;

		uint_t colon = source.Find(":", cbufferStart);

		String name(source.str + cbufferStart, colon - cbufferStart);
		name.RemoveBlankspace();

		uint_t regIndex = source.Find("register", colon) + 10;

		ShaderStructInfo* cbuffer = new ShaderStructInfo;

		cbuffer->name = name;
		cbuffer->semRegister = atoi(*source + regIndex);



		CalcStructSize(source, cbufferStart - 7, cbuffer);

		switch (type) {
			case FD_SHADER_TYPE_VERTEXSHADER:
				vCBuffers.Push_back(cbuffer);
				FD_DEBUG("[ShaderParser] Found vCBuffer <NAME: %s> <SIZE: %u> <SLOT: %u>", *cbuffer->name, cbuffer->structSize, cbuffer->semRegister);
				break;
			case FD_SHADER_TYPE_PIXELSHADER:
				pCBuffers.Push_back(cbuffer);
				FD_DEBUG("[ShaderParser] Found pCBuffer <NAME: %s> <SIZE: %u> <SLOT: %u>", *cbuffer->name, cbuffer->structSize, cbuffer->semRegister);
				break;
		}
	}


}

void Shader::CalcStructSize(String& structSource, uint_t offset, ShaderStructInfo* cbuffer) {

	FD_SHADER_FIELD_TYPE types[6]{
		FD_SHADER_FIELD_TYPE_MAT4,
		FD_SHADER_FIELD_TYPE_MAT3,
		FD_SHADER_FIELD_TYPE_VEC4,
		FD_SHADER_FIELD_TYPE_VEC3,
		FD_SHADER_FIELD_TYPE_VEC2,
		FD_SHADER_FIELD_TYPE_FLOAT };

	uint_t end = structSource.Find("};", offset);

	String fields(*structSource + offset, end - offset);

	uint_t numFields = fields.Count(";");

	uint_t currSemicolon = fields.Find(";");
	uint_t fieldOffset = 0;

	cbuffer->structSize = 0;

	for (uint_t num = 0; num < numFields; num++) {
		for (uint_t i = 0; i < 6; i++) {
			String fieldType = get_field_type_as_string(types[i]);
			uint_t index = fields.Find(fieldType, fieldOffset);

			if (index > currSemicolon || index == -1) continue;

			uint32 fieldSize = get_field_type_size(types[i]);
			cbuffer->structSize += fieldSize;

			cbuffer->layout.PushElement(fields.SubString(index + fieldType.length, fields.Find(";", index + 1)).RemoveBlankspace(), fieldSize);

			fieldOffset = currSemicolon;
			currSemicolon = fields.Find(";", currSemicolon + 1);

			break;
		}
	}

	structSource.Remove(offset, end + 2);
}

void Shader::ParseTextures(String source) {

	while (true) {
		uint_t textureStart = source.Find("Texture");

		if (textureStart < (uint_t)0) break;

		ShaderTextureInfo* tex = new ShaderTextureInfo;
		tex->numTextures = 1;

		String type = source.SubString(textureStart + 7, source.Find(" ", textureStart + 7)).RemoveBlankspace();

		if (type == "1D") {
			tex->type = FD_SHADER_TEXTURE_TYPE_TEXTURE1D;
		} else if (type == "2D ") {
			tex->type = FD_SHADER_TEXTURE_TYPE_TEXTURE2D;
		} else if (type == "3D ") {
			tex->type = FD_SHADER_TEXTURE_TYPE_TEXTURE3D;
		} else if (type == "1DArray") {
			tex->type = FD_SHADER_TEXTURE_TYPE_TEXTURE1D_ARRAY;
		} else if (type == "2DArray") {
			tex->type = FD_SHADER_TEXTURE_TYPE_TEXTURE2D_ARRAY;
		} else if (type == "CubeArray") {
			tex->type = FD_SHADER_TEXTURE_TYPE_TEXTURECUBE_ARRAY;
		} else if (type == "Cube") {
			tex->type = FD_SHADER_TEXTURE_TYPE_TEXTURECUBE;
		} else {
			break;
		}


		uint_t nameStart = source.Find(" ", textureStart);
		uint_t nameEnd = source.Find(":", nameStart);

		tex->name = source.SubString(nameStart, nameEnd).RemoveBlankspace();

		uint_t regStart = source.Find("register(t", nameEnd) + 10;
		uint_t end = source.Find(";", regStart);

		if (regStart < 10) {
			FD_WARNING("[ShaderParser] Texture \"%s\" has not been registered to a texture slot", *tex->name);
		}

		String sem = source.SubString(regStart, end);

		tex->semRegister = atoi(*sem);

		uint_t subCompStart = sem.Find("[");
		if (subCompStart != (uint_t)-1) {
			tex->numTextures = atoi(*sem + subCompStart + 1);
		}

		source.Remove(textureStart, end + 1);

		pTextures.Push_back(tex);
	}

}

}
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
		case FD_SHADER_FIELD_TYPE_FLOAT: return ("float");
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

	//struct
	while (true) {
		uint_t cstructStart = source.Find("struct ") + 7;

		if (cstructStart < 7) break;

		uint_t space = source.Find(" ", cstructStart + 1);

		String name = source.SubString(cstructStart, space).RemoveBlankspace();

		StructDefinition* def = new StructDefinition;

		def->name = name;

		uint_t end = source.Find("};", cstructStart) + 2;

		CalcStructSize(source.SubString(source.Find("{", cstructStart) + 1, end - 2).RemoveChars("\t\n\r", true), &def->structSize, &def->layout, type);

		source.Remove(cstructStart - 7, end);

		switch (type) {
		case FD_SHADER_TYPE_VERTEXSHADER:
			vStructs.Push_back(def);
			FD_DEBUG("[ShaderParser] Found vStruct <NAME: %s> <SIZE: %u>", *name, def->structSize);
			break;
		case FD_SHADER_TYPE_PIXELSHADER:
			pStructs.Push_back(def);
			FD_DEBUG("[ShaderParser] Found pStruct <NAME: %s> <SIZE: %u>", *name, def->structSize);
			break;
		}
	}


	//cbuffer
	while (true) {
		uint_t cbufferStart = source.Find("cbuffer ") + 8;

		if (cbufferStart < 8) break;

		uint_t colon = source.Find(":", cbufferStart);

		String name = source.SubString(cbufferStart, colon).RemoveBlankspace();

		uint_t regIndex = source.Find("register", colon) + 10;

		ShaderStructInfo* cbuffer = new ShaderStructInfo;

		cbuffer->name = name;
		cbuffer->semRegister = (uint32)atoi(*source + regIndex);

		uint_t end = source.Find("};", cbufferStart) + 2;

		CalcStructSize(source.SubString(source.Find("{", cbufferStart)+1, end-2).RemoveChars("\t\n\r", true), &cbuffer->structSize, &cbuffer->layout, type);

		source.Remove(cbufferStart - 8, end);

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

Shader::ShaderStructFieldType Shader::GetStructFieldType(const String& typeName, FD_SHADER_TYPE type) {
	static FD_SHADER_FIELD_TYPE types[6]{
		FD_SHADER_FIELD_TYPE_MAT4,
		FD_SHADER_FIELD_TYPE_MAT3,
		FD_SHADER_FIELD_TYPE_VEC4,
		FD_SHADER_FIELD_TYPE_VEC3,
		FD_SHADER_FIELD_TYPE_VEC2,
		FD_SHADER_FIELD_TYPE_FLOAT
	};

	for (uint_t i = 0; i < 6; i++) {
		if (get_field_type_as_string(types[i]) == typeName) {
			ShaderStructFieldType ret;

			ret.type = FD_STRUCT_FIELD_TYPE_PRIMITVE;
			ret.size = get_field_type_size(types[i]);
			return ret;
		}
	}

	uint_t numStructs = 0;

	if (type == FD_SHADER_TYPE_VERTEXSHADER) {
		numStructs = vStructs.GetSize();

		for (uint_t i = 0; i < numStructs; i++) {
			StructDefinition* def = vStructs[i];

			if (def->name == typeName) {
				ShaderStructFieldType ret;
				ret.type = FD_STRUCT_FIELD_TYPE_STRUCT;
				ret.size = def->structSize;
				ret.layout = def->layout;

				return ret;
			}
		}
	}

	if (type == FD_SHADER_TYPE_PIXELSHADER) {
		numStructs = pStructs.GetSize();

		for (uint_t i = 0; i < numStructs; i++) {
			StructDefinition* def = pStructs[i];

			if (def->name == typeName) {
				ShaderStructFieldType ret;
				ret.type = FD_STRUCT_FIELD_TYPE_STRUCT;
				ret.size = def->structSize;
				ret.layout = def->layout;

				return ret;
			}
		}
	}

	ShaderStructFieldType ret;
	ret.type = FD_STRUCT_FIELD_TYPE_UNKNOWN;
	ret.size = (uint32)-1;

	return ret;
}

void Shader::CalcStructSize(String fields, uint32* size, BufferLayout* layout, FD_SHADER_TYPE shaderType) {

	uint_t numFields = fields.Count(";");

	uint_t currSemicolon = fields.Find(";");
	uint_t fieldOffset = 0;

	*size = 0;

	for (uint_t num = 0; num < numFields; num++) {
		String typeName = fields.SubString(fieldOffset, fields.Find(" ", fieldOffset)).RemoveBlankspace();
		ShaderStructFieldType type = GetStructFieldType(typeName, shaderType);

		if (type.type == FD_STRUCT_FIELD_TYPE_UNKNOWN && type.size == (uint32)-1) {
			FD_FATAL("[ShaderParser] Unknown type referenced, \"%s\"", *typeName);
			return;
		}

		String name = fields.SubString(fieldOffset + typeName.length, currSemicolon).RemoveBlankspace();

		if (type.type == FD_STRUCT_FIELD_TYPE_PRIMITVE) {
			layout->PushElement(name, type.size);
			*size += type.size;
		} else {
			layout->PushElementAtOffset(name, type.size, *size);

			*size += type.size;

			const List<BufferLayout::BufferLayoutAttrib*>& attribs = type.layout.GetElements();

			for (uint_t i = 0; i < attribs.GetSize(); i++) {
				layout->PushElement(name + "." + attribs.Get(i)->name, attribs.Get(i)->size);
			}
		}

		fieldOffset = currSemicolon+1;
		currSemicolon = fields.Find(";", currSemicolon + 1);	
	}
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
		} else if (type == "2D") {
			tex->type = FD_SHADER_TEXTURE_TYPE_TEXTURE2D;
		} else if (type == "3D") {
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
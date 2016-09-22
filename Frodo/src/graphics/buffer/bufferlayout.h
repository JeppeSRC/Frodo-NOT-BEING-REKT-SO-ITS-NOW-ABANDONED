#pragma once
#include <fd.h>
#include <core/log.h>
#include <graphics/d3dcontext.h>
#include <util/list.h>
#include <util/string.h>
#include <math/math.h>

class FDAPI BufferLayout {
private:
	friend class Shader;
private:
	struct BufferLayoutAttrib {
		String name;
		DXGI_FORMAT format;
		unsigned int slot;
		unsigned int size;
		unsigned int offset;
	};

private:
	List<BufferLayoutAttrib> elements;

	unsigned int offset;

	void Push(const String& name, DXGI_FORMAT format, unsigned int slot);

public:
	BufferLayout() { offset = 0; elements.Reserve(32); }
	~BufferLayout();

	void CreateInputLayout(Shader* shader);

	template<typename T>
	inline void Push(const char* name, unsigned int slot = 0) { FD_FATAL("Unsupported format"); }

	template<> inline void Push<float>(const char* name, unsigned int slot) { Push(name, DXGI_FORMAT_R32_FLOAT, slot); }
	template<> inline void Push<int>(const char* name, unsigned int slot) { Push(name, DXGI_FORMAT_R8G8B8A8_SNORM, slot); }
	template<> inline void Push<unsigned int>(const char* name, unsigned int slot) { Push(name, DXGI_FORMAT_R8G8B8A8_UNORM, slot); }
	template<> inline void Push<vec2>(const char* name, unsigned int slot) { Push(name, DXGI_FORMAT_R32G32_FLOAT, slot); }
	template<> inline void Push<vec3>(const char* name, unsigned int slot) { Push(name, DXGI_FORMAT_R32G32B32_FLOAT, slot); }
	template<> inline void Push<vec4>(const char* name, unsigned int slot) { Push(name, DXGI_FORMAT_R32G32B32A32_FLOAT, slot); }
	
};

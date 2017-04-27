#pragma once
#include <fd.h>
#include <core/log.h>
#include <graphics/d3dcontext.h>
#include <util/list.h>
#include <util/string.h>
#include <math/math.h>

namespace FD {

class FDAPI BufferLayout {
private:
	friend class Shader;
public:
	struct BufferLayoutAttrib {
		String name;
		DXGI_FORMAT format;
		uint32 slot;
		uint32 size;
		uint32 offset;
	};

private:
	List<BufferLayoutAttrib*> elements;

	uint32 offset;

	void Push(const String& name, DXGI_FORMAT format, uint32 slot);

public:
	BufferLayout() { offset = 0; elements.Reserve(32); }
	~BufferLayout();

	void CreateInputLayout(Shader* shader);

	void PushElement(const String& name, uint32 size);
	void PushElementAtOffset(const String& name, uint32 size, uint32 offset);
	uint32 GetElementOffset(const String& name) const;
	uint32 GetElementOffset(uint32 index) const;
	uint32 GetElementSize(const String& name) const;
	uint32 GetElementSize(uint32 index) const;
	uint32 GetSize() const { return offset; }

	inline const List<BufferLayoutAttrib*>& GetElements() const { return elements; }

	template<typename T>
	inline void Push(const char* name, uint32 slot = 0) { FD_FATAL("Unsupported format"); }

	template<> inline void Push<float32>(const char* name, uint32 slot) { Push(name, DXGI_FORMAT_R32_FLOAT, slot); }
	template<> inline void Push<int32>(const char* name, uint32 slot) { Push(name, DXGI_FORMAT_R8G8B8A8_SNORM, slot); }
	template<> inline void Push<uint32>(const char* name, uint32 slot) { Push(name, DXGI_FORMAT_R8G8B8A8_UNORM, slot); }
	template<> inline void Push<vec2>(const char* name, uint32 slot) { Push(name, DXGI_FORMAT_R32G32_FLOAT, slot); }
	template<> inline void Push<vec3>(const char* name, uint32 slot) { Push(name, DXGI_FORMAT_R32G32B32_FLOAT, slot); }
	template<> inline void Push<vec4>(const char* name, uint32 slot) { Push(name, DXGI_FORMAT_R32G32B32A32_FLOAT, slot); }

};

}
#pragma once

#include <Windows.h>
#include <fd.h>
#include <util/string.h>
#include <util/list.h>
#include <util/map.h>

#define FD_VFS_MAX_MOUNT_POINTS 128

class FDAPI VFS {
private:
	static VFS* instance;

public:

	static inline void Init() { VFS::instance = new VFS(); }
	static inline void Dispose() { delete VFS::instance; }

	static __forceinline VFS* Get() { return VFS::instance; }

private:
	Map<String, String> mountPoints;
	
	String ResolvePath(const String& vpath);

public:
	VFS();
	~VFS();

	void Mount(const String& name, const String& path);
	void UnMount(const String& name);
	unsigned char* ReadFile(const String& filename, size_t* fileSize = nullptr);
	String ReadTextFile(const String& filename);
	

};

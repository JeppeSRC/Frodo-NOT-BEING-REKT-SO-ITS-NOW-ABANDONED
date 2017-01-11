#include "vfs.h"
#include <core/log.h>
#include <util/fileutils.h>

VFS* VFS::instance = nullptr;

VFS::VFS() {
	mountPoints.Reserve(FD_VFS_MAX_MOUNT_POINTS);
}

VFS::~VFS() {
}

String VFS::ResolvePath(const String& vpath) {

	if (!vpath.StartsWith("/")) {
		return vpath;
	}

	List<String*> list = vpath.Split('/');

	String vmountPath = list[1];
	String filename = list[list.GetSize() - 1];

	String mountPath = mountPoints.Retrieve(vmountPath);

	return String(mountPath + filename);
}

void VFS::Mount(const String& name, const String& path) {
	mountPoints.Add(path, name);
}

void VFS::UnMount(const String& name) {
	mountPoints.Remove(name);
}

byte* VFS::ReadFile(const String& filename, uint_t* fileSize) {
	uint_t* fsize = fileSize != nullptr ? fileSize : new uint_t(0);
	return FDReadBinaryFile(ResolvePath(filename), fsize);
}

String VFS::ReadTextFile(const String& filename) {
	return FDReadTextFile(ResolvePath(filename));
}

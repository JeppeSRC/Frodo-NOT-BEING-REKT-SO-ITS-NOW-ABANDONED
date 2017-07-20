#include "vfs.h"
#include <core/log.h>
#include <util/fileutils.h>

namespace FD {

VFS* VFS::instance = nullptr;

VFS::VFS() {
	mountPoints.Reserve(FD_VFS_MAX_MOUNT_POINTS);
}

VFS::~VFS() {}

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
	mountPoints.Add(name, path);
}

void VFS::UnMount(const String& name) {
	mountPoints.Remove(name);
}

byte* VFS::ReadFile(const String& filename, uint_t* fileSize) {
	return FDReadBinaryFile(ResolvePath(filename), fileSize);
}

String VFS::ReadTextFile(const String& filename) {
	return FDReadTextFile(ResolvePath(filename));
}

}
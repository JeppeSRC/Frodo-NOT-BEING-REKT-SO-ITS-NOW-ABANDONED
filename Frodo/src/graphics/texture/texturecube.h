#pragma once

#include "texture.h"

namespace FD {


class FDAPI TextureCube : public Texture {
private:
	void LoadSingleFile(const String& filename);
	void LoadMultipleFiles(const String* files);

public:
	//ORDER: X+ X- Y+ Y- Z+ Z-
	TextureCube(const String* files);
	~TextureCube();

	void Bind(uint32 slot = 0) const override;
};
}

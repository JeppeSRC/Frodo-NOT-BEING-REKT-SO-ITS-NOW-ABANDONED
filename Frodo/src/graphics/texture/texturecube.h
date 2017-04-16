#pragma once

#include "texture.h"

namespace FD {


class FDAPI TextureCube : public Texture {
protected:
	ID3D11Texture2D* resource = nullptr;

private:
	void LoadSingleFile(const String& filename);
	void LoadMultipleFiles(const String* files);

public:
	TextureCube(const String* files);
	~TextureCube();

	void Bind(unsigned int slot = 0) override;

	inline ID3D11Texture2D* GetResource() const { return resource; }
};
}

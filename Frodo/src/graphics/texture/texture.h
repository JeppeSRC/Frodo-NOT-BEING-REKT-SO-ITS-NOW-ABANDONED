#pragma once

#include <fd.h>
#include <graphics/d3dcontext.h>

class FDAPI FDTexture {
protected:
	unsigned int width;
	unsigned int height;

public:

	virtual void Bind() = 0;

	inline unsigned int GetWidth() const { return width; }
	inline unsigned int GetHeight() const { return height; }
};

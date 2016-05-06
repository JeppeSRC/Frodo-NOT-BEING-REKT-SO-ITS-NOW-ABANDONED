#pragma once

#include <core/log.h>
#include <core/window.h>

#include <graphics/d3dcontext.h>
#include <graphics/buffer/bufferlayout.h>
#include <graphics/buffer/vertexbuffer.h>
#include <graphics/buffer/indexbuffer.h>
#include <graphics/shader/shader.h>
#include <graphics/texture/texture2d.h>
#include <graphics/texture/framebuffer2d.h>

#include <math/math.h>
#include <math/mathcommon.h>

#include <util/fileutils.h>
#include <util/list.h>
#include <util/map.h>
#include <util/string.h>

#include <fd.h>

class FDAPI FDApplication {
private:
	void Run();
	float capUpdate;

protected:
	Window* window;

	virtual void OnInit();
	virtual void OnUpdate(float delta);
	virtual void OnRender();
	virtual void OnExit();

public:
	FDApplication(const char* title, unsigned int width, unsigned int height);
	~FDApplication();


	inline void start() { Run(); }
};

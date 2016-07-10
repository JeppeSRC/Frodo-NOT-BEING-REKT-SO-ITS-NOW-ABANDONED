#pragma once

#include <core/log.h>
#include <core/window.h>
#include <core/input.h>

#include <graphics/d3dcontext.h>
#include <graphics/buffer/bufferlayout.h>
#include <graphics/buffer/vertexbuffer.h>
#include <graphics/buffer/indexbuffer.h>

#include <graphics/shader/shader.h>
#include <graphics/shader/shaderfactory.h>

#include <graphics/texture/texture2d.h>
#include <graphics/texture/framebuffer2d.h>
#include <graphics/texture/texturemanager.h>

#include <graphics/render/deferredrenderer.h>

#include <graphics/render/material/material.h>
#include <graphics/render/model/model.h>
#include <graphics/render/model/meshfactory.h>

#include <entity/entity.h>

#include <math/math.h>
#include <math/mathcommon.h>

#include <util/fileutils.h>
#include <util/list.h>
#include <util/map.h>
#include <util/string.h>
#include <util/vfs/vfs.h>

#include <fd.h>

class FDAPI FDApplication {
private:
	void Run();

	float ups;

protected:
	Window* window;

	virtual void OnInit();
	virtual void OnTick();
	virtual void OnUpdate(float delta);
	virtual void OnRender();
	virtual void OnExit();

public:
	FDApplication(const char* title, unsigned int width, unsigned int height);
	virtual ~FDApplication();


	inline void start() { Run(); }

	inline void SetUPS(float timesPerSec) { this->ups = 1000.0f / timesPerSec; }
};

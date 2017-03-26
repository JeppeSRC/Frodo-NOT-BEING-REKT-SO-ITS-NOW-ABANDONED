#pragma once

#include <core/log.h>
#include <core/window.h>
#include <core/input.h>

#include <core/ui/ui.h>

#include <core/event/event.h>
#include <core/event/eventdispatcher.h>
#include <core/event/eventlistener.h>
#include <core/event/eventkayboard.h>
#include <core/event/eventmouse.h>
#include <core/event/eventwindow.h>

#include <graphics/d3dcontext.h>
#include <graphics/buffer/bufferlayout.h>
#include <graphics/buffer/vertexbuffer.h>
#include <graphics/buffer/indexbuffer.h>

#include <graphics/shader/shader.h>
#include <graphics/shader/shaderfactory.h>

#include <graphics/texture/texture2d.h>
#include <graphics/texture/framebuffer2d.h>
#include <graphics/texture/texturemanager.h>

#include <graphics/font/font.h>

/*#include <graphics/render/deferredrenderer.h>
#include <graphics/render/forwardrenderer.h>*/
#include <graphics/render/renderer/fontrenderer.h>
#include <graphics/render/renderer/batchrenderer.h>
#include <graphics/render/renderer/renderer.h>
#include <graphics/render/renderer/menurenderer.h>


#include <graphics/render/material/material.h>
#include <graphics/render/mesh/meshfactory.h>
#include <graphics/render/camera/camera.h>
#include <graphics/render/camera/usercamera.h>

#include <graphics/render/light/light.h>

#include <entity/entity.h>

#include <math/math.h>
#include <math/mathcommon.h>

#include <util/fileutils.h>
#include <util/list.h>
#include <util/map.h>
#include <util/string.h>

#include <util/vfs/vfs.h>

#include <util/asset/package.h>
#include <util/asset/asset.h>
#include <util/asset/assetmanager.h>

#include <fd.h>

namespace FD {

class FDAPI FDApplication {
private:
	void Run();

	float32 ups;

protected:
	Window* window;
	Renderer* mainRenderer;

	virtual void OnInit();
	virtual void OnTick();
	virtual void OnUpdate(float32 delta);
	virtual void OnRender();
	virtual void OnExit();

public:
	FDApplication(const char* title, uint32 width, uint32 height);
	virtual ~FDApplication();


	inline void Start() { Run(); }

	inline void SetUPS(float32 timesPerSec) { this->ups = 1000.0f / timesPerSec; }
};

}
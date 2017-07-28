#pragma once

#include <audio/audio.h>
#include <audio/audiomanager.h>
#include <audio/xadevice.h>
#include <audio/audiomixer.h>

#include <fdutils.h>
#include <core/window.h>
#include <core/input.h>

#include <core/ui/ui.h>

#include <core/event/event.h>
#include <core/event/eventdispatcher.h>
#include <core/event/eventlistener.h>
#include <core/event/eventkeyboard.h>
#include <core/event/eventmouse.h>
#include <core/event/eventwindow.h>

#include <core/event/fd_key.h>

#include <core/scene.h>

#include <graphics/d3dcontext.h>
#include <graphics/d3dadapter.h>
#include <graphics/d3dfactory.h>
#include <graphics/d3doutput.h>

#include <graphics/buffer/bufferlayout.h>
#include <graphics/buffer/vertexbuffer.h>
#include <graphics/buffer/indexbuffer.h>

#include <graphics/shader/shader.h>
#include <graphics/shader/shaderfactory.h>

#include <graphics/texture/texture2d.h>
#include <graphics/texture/texturecube.h>
#include <graphics/texture/framebuffer2d.h>
#include <graphics/texture/framebuffercube.h>
#include <graphics/texture/shadowmap2d.h>
#include <graphics/texture/shadowmapcube.h>
#include <graphics/texture/texturemanager.h>

#include <graphics/font/font.h>

#include <graphics/pbr/render/pbrrenderer.h>
/*#include <graphics/render/deferredrenderer.h>
#include <graphics/render/forwardrenderer.h>*/
#include <graphics/render/renderer/simplerenderer.h>
#include <graphics/render/renderer/fontrenderer.h>
#include <graphics/render/renderer/batchrenderer.h>
#include <graphics/render/renderer/renderer.h>
#include <graphics/render/renderer/menurenderer.h>
#include <graphics/render/renderer/spriterenderer.h>


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

class FDAPI Application {
private:
	void Run();

	float32 ups;

protected:
	Window* window;

	virtual void OnCreateWindow() = 0;
	virtual void OnInit() = 0;
	virtual void OnTick();
	virtual void OnUpdate(float32 delta);
	virtual void OnRender();
	virtual void OnExit();

	Application();
public:
	virtual ~Application();


	inline void Start() { Run(); }

	inline void SetUPS(float32 timesPerSec) { this->ups = timesPerSec == 0 ? 0.0f : 1000.0f / timesPerSec; }
};

}
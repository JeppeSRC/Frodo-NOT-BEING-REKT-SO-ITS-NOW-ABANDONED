#include <stdio.h>
#include <util/list.h>
#include <util/map.h>
#include <util/string.h>
#include <core/log.h>
#include <core/window.h>
#include <graphics/buffer/vertexbuffer.h>

int main() {
	FD_WARNING("sd");

	FD_ASSERT(false);

	Window w("Title", 600, 400);
	w.SetVisible(true);
	
	Map<String, float> map(2);

	map["Hello"] = 10.0f;
	map["World"] = 20.0f;

	FD_DEBUG("%f %f", map["Hello"], map["World"]);

	float a[3]{0, 2, 3};

	FDVertexBuffer b(&a, sizeof(a), 12);

	while (w.IsOpen()) {
		w.Clear();



		w.SwapBuffers();
	}

	return 0;
}
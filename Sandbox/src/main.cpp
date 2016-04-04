#include <stdio.h>
#include <util/list.h>
#include <util/map.h>
#include <util/string.h>
#include <core/log.h>
#include <core/window.h>
#include <graphics/buffer/vertexbuffer.h>
#include <graphics/buffer/indexbuffer.h>

int main() {
	Window w("Title", 600, 400);
	w.SetVisible(true);
	
	Map<String, float> map(2);

	map["Hello"] = 10.0f;
	map["World"] = 20.0f;

	FD_DEBUG("%f %f", map["Hello"], map["World"]);

	float a[]{
		0, 1, 0,
		1, -1, 0,
		-1, -1, 0
	};

	unsigned int indices[] = {0, 1, 2};

	FDVertexBuffer v(&a, sizeof(a), 12);
	FDIndexBuffer i(indices, 3);

	while (w.IsOpen()) {
		w.Clear();



		w.SwapBuffers();
	}

	return 0;
}
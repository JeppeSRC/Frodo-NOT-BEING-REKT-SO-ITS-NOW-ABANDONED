#include <stdio.h>
#include <util/list.h>
#include <util/map.h>
#include <util/string.h>
#include <core/log.h>
#include <core/window.h>
#include <graphics/buffer/vertexbuffer.h>
#include <graphics/buffer/indexbuffer.h>
#include <graphics/texture/texture2d.h>

int main() {
	Window w("Title", 600, 400);
	w.SetVisible(true);

	float a[]{
		0, 1, 0,
		1, -1, 0,
		-1, -1, 0
	};

	unsigned int indices[] = {0, 1, 2};

	FDVertexBuffer v(&a, sizeof(a), 12);
	FDIndexBuffer i(indices, 3);

	float pix[4*4]{
		1, 0, 1, 1,
		1, 0, 1, 1,
		1, 0, 1, 1,
		1, 0, 1, 1
	};

	FDTexture2D texture(pix, 2, 2, FD_TEXTURE2D_FORMAT_FLOAT_32_32_32_32);

	while (w.IsOpen()) {
		w.Clear();



		w.SwapBuffers();
	}

	return 0;
}
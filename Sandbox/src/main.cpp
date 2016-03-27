#include <stdio.h>
#include <util/list.h>
#include <util/map.h>
#include <util/string.h>
#include <core/log.h>
#include <core/window.h>

int main() {

	Window w("Title", 600, 400);
	w.SetVisible(true);
	
	Map<String, float> map(2);

	map["Hello"] = 10.0f;
	map["World"] = 20.0f;

	FD_DEBUG("%f %f", map["Hello"], map["World"]);

	while (w.IsOpen()) {
		w.Clear();



		w.SwapBuffers();
	}

	return 0;
}
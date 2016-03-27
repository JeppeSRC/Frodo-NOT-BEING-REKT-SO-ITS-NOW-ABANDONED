#include <stdio.h>
#include <util/list.h>
#include <util/string.h>
#include <core/log.h>
#include <core/window.h>

int main() {

	Window w("Title", 600, 400);
	w.SetVisible(true);
	

	while (w.IsOpen()) {
		w.Clear();



		w.SwapBuffers();
	}

	return 0;
}
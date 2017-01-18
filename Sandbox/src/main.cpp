#include <stdio.h>
#include "test.h"
#include "deferredtest.h"
#include <core/log.h>
#include <string>
#include <util/vfs/vfs.h>

#define PRINT(str) FD_INFO("%s", *str)

int main() {
	Test t;
	t.Start();

/*	DeferredTest g;
	g.Start();*/

	return 0;
}
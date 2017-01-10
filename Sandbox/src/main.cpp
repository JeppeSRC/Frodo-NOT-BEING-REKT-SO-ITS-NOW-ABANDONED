#include <stdio.h>
#include "test.h"
#include "deferredtest.h"
#include "pbrtest.h"
#include <core/log.h>
#include <string>
#include <util/vfs/vfs.h>

#define PRINT(str) FD_INFO("%s", *str)

int main() {

	PBRTest test;
	test.start();

/*
	DeferredTest g;
	g.start();*/

	return 0;
}
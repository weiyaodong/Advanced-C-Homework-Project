#include "winsgl.h"

#pragma comment(lib, SG_LIB("winsgl"))

void sgSetup() {
	initWindow(1360,960, "SGL sample", BIT_MAP);
}
void sgLoop() {
	setColor(100, 100, 100);
	clearScreen();
	return;
}
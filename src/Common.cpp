#include "Common.h"

_Global Global;


void schedualWindowDisposal(nanogui::Window* window) {
	nanogui::async([window]() {
		window->dispose();
	});
};
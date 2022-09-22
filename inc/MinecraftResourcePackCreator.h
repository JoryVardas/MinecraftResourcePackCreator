#pragma once

#include <nanogui/nanogui.h>
#include "MinecraftResourcePack/MinecraftResourcePack.h"

class MinecraftResourcePackCreatorApp : public nanogui::Screen {
public:
	MinecraftResourcePackCreatorApp();
private:
	const int _topMenuHeight = 30;

	nanogui::Window* _topMenu;
	nanogui::Window* _structureBrowser;
	nanogui::Window* _objectView;

	MinecraftResourcePack _workingResourcePack;

	friend MinecraftResourcePack;
};
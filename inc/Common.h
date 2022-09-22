#pragma once

#include <nanogui/nanogui.h>

#include "Error.h"

class MinecraftResourcePackCreatorApp;

struct _Global {
	const unsigned int _majorVersion = 0;
	const unsigned int _minorVersion = 1;

	const std::string _ProjectLoaderString = "Minecraft Resource Pack Project Loader";
	const std::string _ProjectCreatorString = "Minecraft Resource Pack Project Creator";

	nanogui::ref<MinecraftResourcePackCreatorApp> _app;
};

extern _Global Global;



void schedualWindowDisposal(nanogui::Window* window);
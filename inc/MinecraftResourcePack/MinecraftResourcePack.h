#pragma once

#include <vector>
#include <string>
#include <filesystem>

#include "Common.h"

#include "BlockState.h"
#include "ui/LoadingPopup.h"

class MinecraftResourcePack {
public:
	void loadDefault(nanogui::ref<LoadingPopup> loadingPopup = nullptr);
	void load(std::filesystem::path projectFilePath, nanogui::ref<LoadingPopup> loadingPopup = nullptr);

	void newPack(std::filesystem::path projectFilePath, nanogui::ref<LoadingPopup> loadingPopup = nullptr);

	std::string packName() const;
private:
	//std::vector<BlockState> _blockstates;
	std::filesystem::path _rootFolder;
	std::filesystem::path _assetFolder;
	std::filesystem::path _projectFile;
};
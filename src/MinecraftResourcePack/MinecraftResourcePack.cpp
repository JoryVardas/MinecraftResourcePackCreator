#include "MinecraftResourcePack/MinecraftResourcePack.h"
#include "ui/LoadingPopup.h"
#include "MinecraftResourcePackCreator.h"
#include "ui/TreeView.h"

#include <nlohmann\json.hpp>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <thread>
#include <future>

using json = nlohmann::json;
using namespace std;

void updateLoadingPopup(nanogui::ref<LoadingPopup> loadingPopup, const std::string& message, float percentToAdd) {
	if (!loadingPopup) return;
	nanogui::async([loadingPopup, message, percentToAdd]() {
		nanogui::ref<LoadingPopup> popup = loadingPopup;
		popup->setMessage(message);
		popup->setProgress(popup->progress() + (percentToAdd / 100));
	});
};
void updateLoadingPopup(nanogui::ref<LoadingPopup> loadingPopup, const std::string& message, float percent, bool override) {
	if (!loadingPopup) return;
	nanogui::async([loadingPopup, message, percent]() {
		nanogui::ref<LoadingPopup> popup = loadingPopup;
		popup->setMessage(message);
		popup->setProgress((percent / 100));
	});
};

void MinecraftResourcePack::loadDefault(nanogui::ref<LoadingPopup> loadingPopup) {
	load("res/default_project.json", loadingPopup);
};

void MinecraftResourcePack::load(std::filesystem::path projectFilePath, nanogui::ref<LoadingPopup> loadingPopup) {

	auto unloadCallback = [loadingPopup]() {
		auto popup = loadingPopup;
		schedualWindowDisposal(popup);
	};

	updateLoadingPopup(loadingPopup, "Loading project file.", 0, true);

	ifstream defaultFile(projectFilePath);

	if (!defaultFile.good()) {
		defaultFile.close();
		throwWarning(Global._ProjectLoaderString, "The project file (" + projectFilePath.string() + ") could not be loaded.\nThe project will be unloaded.", unloadCallback);
		return;
	}


	json project;
	defaultFile >> project;

	defaultFile.close();

	updateLoadingPopup(loadingPopup, "Reading project data.", 1);

	if (!project.contains("mrpc_version")) {
		throwWarning(Global._ProjectLoaderString, "The selected project is missing a version.\nThe project will be unloaded.", unloadCallback);
		return;
	}

	auto& projectVersion = project.at("mrpc_version");

	if (!projectVersion.contains("major") || !projectVersion.contains("minor")) {
		throwWarning(Global._ProjectLoaderString, "The selected project's version info could not be read.\nThe project will be unloaded.", unloadCallback);
		return;
	}

	unsigned int projectMajorVersion = 0;
	projectVersion.at("major").get_to(projectMajorVersion);

	unsigned int projectMinorVersion = 0;
	projectVersion.at("minor").get_to(projectMinorVersion);

	if (projectMajorVersion != Global._majorVersion || projectMinorVersion != Global._minorVersion) {
		throwWarning(Global._ProjectLoaderString, "The selected project's version is different than than current app version.\nThe project will be unloaded.", unloadCallback);
		return;
	}


	_projectFile = projectFilePath;
	_rootFolder = projectFilePath;
	_rootFolder.replace_extension();
	_assetFolder = _rootFolder / "assets";

	updateLoadingPopup(loadingPopup, "Loading default block list.", 0.5);

	auto addStructureBrowserNode = []<typename Parent>(Parent* parent, string text, int icon) -> TreeViewItem* {
		promise<TreeViewItem*> creationPromise;
		future<TreeViewItem*> creationFuture = creationPromise.get_future();
		nanogui::async([parent, &text, icon, &creationPromise]() {
			auto parentNonConst = parent;
			auto newNode = new TreeViewItem(parentNonConst, text, icon);
			auto containingTreeView = const_cast<TreeView*>(newNode->getContainingTreeView());
			containingTreeView->requireLayoutUpdate();
			//newNode->set_visible(true);
			creationPromise.set_value(newNode);
		});

		creationFuture.wait();
		return creationFuture.get();
	};

	auto rootNode = dynamic_cast<TreeView*>(Global._app->_structureBrowser->child_at(0))->getRootItem();
	Global._app->_structureBrowser->set_visible(true);
	auto projectNode = addStructureBrowserNode(rootNode, packName(), FA_PROJECT_DIAGRAM);
	
	ifstream blockListFile("res/block_list.json");

	if (!blockListFile.good()) {
		blockListFile.close();
		throwWarning(Global._ProjectLoaderString, "The block list file could not be loaded.\nThe project will be unloaded.", unloadCallback);
		return;
	}


	json blockList;
	blockListFile >> blockList;

	blockListFile.close();

	updateLoadingPopup(loadingPopup, "Reading block list.", 0.5);

	auto readGroups = [&addStructureBrowserNode,&unloadCallback](auto&& self, json& containingArray, TreeViewItem* container) -> void {
		for (auto& group : containingArray) {
			if (!group.contains("name")) {
				throwWarning(Global._ProjectLoaderString, "The default block list was ill-formed.\nThe project will be unloaded.", unloadCallback);
			}
			std::string groupName;
			group.at("name").get_to(groupName);
			auto groupNode = addStructureBrowserNode(container, groupName, FA_CUBES);

			if (group.contains("subgroups")) {
				json subGroups;
				group.at("subgroups").get_to(subGroups);

				self(self, subGroups, groupNode);
			}

			if (group.contains("blocks")) {
				vector<string> blocks;
				group.at("blocks").get_to(blocks);
				for (auto& block : blocks) {
					addStructureBrowserNode(groupNode, block, FA_CUBE);
				}
			}
		}
	};

	readGroups(readGroups, blockList, projectNode);

		schedualWindowDisposal(loadingPopup);
};

void MinecraftResourcePack::newPack(std::filesystem::path projectFilePath, nanogui::ref<LoadingPopup> loadingPopup) {

#define ifFileErrorThrowAndReturn(error) 	if (error){\
	schedualWindowDisposal(loadingPopup);\
	throwWarning(Global._ProjectCreatorString, "There was an error while trying to access the filesystem.  The project was not created.");\
	std::cerr << "Filesystem error: " << error.message();\
	return;\
}

	using namespace std::filesystem;

	_rootFolder = projectFilePath;
	_rootFolder.replace_extension();
	_assetFolder = _rootFolder / "assets";
	_projectFile = projectFilePath;

	updateLoadingPopup(loadingPopup, "Creating resource pack folder structure.", 0, true);

	error_code fileError;

	if (exists(projectFilePath, fileError)) {

		std::promise<int> choicePromise;
		std::future<int> choiceFuture = choicePromise.get_future();

		throwWarningWithChoice(Global._ProjectCreatorString, "The specified project already exists, would you like to load it instead?", "YES", "CANCEL", [this, &choicePromise](int choice) {
			choicePromise.set_value(choice);
		});

		choiceFuture.wait();

		switch (choiceFuture.get()) {
			case 0:
				load(projectFilePath, loadingPopup);
				return;
			case 1:
				schedualWindowDisposal(loadingPopup);
				throwWarning(Global._ProjectCreatorString, "Project already existed, no project was loaded.");
				return;
		}
	}

	ifFileErrorThrowAndReturn(fileError);

	if (exists(_rootFolder)) {

		std::promise<int> choicePromise;
		std::future<int> choiceFuture = choicePromise.get_future();

		throwWarningWithChoice(Global._ProjectCreatorString, "There is already a resource pack with the specified name, would you like to turn it into a Minecraft Resource Pack Project?", "YES", "CANCEL", [this, &choicePromise](int choice) {
			choicePromise.set_value(choice);
		});

		choiceFuture.wait();

		switch (choiceFuture.get()) {
			case 0:
				copy("res/default_project.json", projectFilePath, fileError);

				ifFileErrorThrowAndReturn(fileError);

				updateLoadingPopup(loadingPopup, "", 100);
				load(projectFilePath, loadingPopup);
				return;
			case 1:
				schedualWindowDisposal(loadingPopup);
				throwWarning(Global._ProjectCreatorString, "Project creation failed, no project was loaded.");
				return;
		}
	}

	ifFileErrorThrowAndReturn(fileError);

	auto createAllOrNone = [](auto&& self, vector<path>::const_iterator& begin, const vector<path>::const_iterator& end) -> int {
		if (begin == end) return 0;

		path cur = *begin;

		error_code error;
		create_directory(cur, error);
		if (error) {
			throwWarning(Global._ProjectCreatorString, "Could not create directory.\n\t" + cur.string() + "\n\t" + error.message());
			std::cerr << "Could not create directory." << std::endl << "\t" << cur.string() << std::endl << "\t" << error.message() << std::endl;
			return 1;
		}

		switch (self(self, ++begin, end)) {
			case 0:
				return 0;
			case 1:
				remove(cur, error);
				if (error) {
					throwWarning(Global._ProjectCreatorString, "Could not create directory.\n\t" + cur.string() + "\n\t" + error.message());
					std::cerr << "Could not remove directory." << std::endl << "\t" << cur.string() << std::endl << "\t" << error.message() << std::endl;
					std::cerr << "No further cleanup will be attempted." << std::endl;
					return 2;
				}
				else {
					return 1;
				}
			case 2:
				return 2;
		}

		throwWarning(Global._ProjectCreatorString, "An unknown error occured while trying to create directory.");
		std::cerr << "An unknown error occured while trying to create directory." << std::endl << "No cleanup will be attempted." << std::endl;
		return 2;
	};

	vector<path> pathsToCreate = {
		_rootFolder,
		_assetFolder,
		_assetFolder / "minecraft",
		_assetFolder / "minecraft" / "blockstates",
		_assetFolder / "minecraft" / "models",
		_assetFolder / "minecraft" / "textures"
	};

	switch (createAllOrNone(createAllOrNone, cbegin(pathsToCreate), cend(pathsToCreate))) {
		case 1:
			schedualWindowDisposal(loadingPopup);
			throwWarning(Global._ProjectCreatorString, "There was an error creating the project file structure, any created folders were removed.");
			return;
		case 2:
			schedualWindowDisposal(loadingPopup);
			throwWarning(Global._ProjectCreatorString, "There was an error creating the project file structure, another error occured while trying to remove the created folders.");
			return;
		case 0:
			break;
	}

	updateLoadingPopup(loadingPopup, "Creating project file", 50);

	copy("res/default_project.json", _projectFile, fileError);

	ifFileErrorThrowAndReturn(fileError);

	updateLoadingPopup(loadingPopup, "", 50);

	load(_projectFile, loadingPopup);
	return;
};

std::string MinecraftResourcePack::packName() const {
	return _projectFile.stem().string();
};
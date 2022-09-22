// MinecraftResourcePackCreator.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <thread>
#include <filesystem>

#include "MinecraftResourcePackCreator.h"
#include "ui/NonMoveableWindow.h"
#include "ui/TreeView.h"
#include "ui/GlViewer.h"
#include "ui/LoadingPopup.h"

using namespace nanogui;

MinecraftResourcePackCreatorApp::MinecraftResourcePackCreatorApp() : Screen(Vector2i(900, 600), "Minecraft Resource Pack Creator") {
	//Set up the top menu

	_topMenu = new NonMoveableWindow(this, "");
	_topMenu->set_position({ 0,0 });
	_topMenu->set_layout(new GridLayout(Orientation::Horizontal, 4));

	Button* newButton = new Button(_topMenu, "", FA_FILE);
	newButton->set_icon_position(Button::IconPosition::LeftCentered);
	newButton->set_fixed_height(this->_topMenuHeight);
	newButton->set_tooltip("New");
	newButton->set_callback([this](){
		//auto namePopup = new Window(this, "New Project Name");
		//namePopup->set_layout(new BoxLayout(Orientation::Vertical, Alignment::Middle, 10, 10));
		//namePopup->set_modal(true);
		//namePopup->set_visible(true);
		//auto namePopupClose = new Button(namePopup->button_panel(), "", FA_WINDOW_CLOSE);
		//namePopupClose->set_callback([namePopup]() {
		//	namePopup->dispose();
		//});

		//auto namePopupName = new TextBox(namePopup, "");
		//namePopupName->set_fixed_width(500);
		//namePopupName->set_editable(true);
		//namePopupName->request_focus();

		//auto namePopupCreate = new Button(namePopup, "Create");
		//namePopupCreate->set_callback([this, namePopupName]() {
		//	std::filesystem::path test("res/default_project.json");
		//	if (std::filesystem::exists(test)) {
		//		std::cout << "File exists" << std::endl;
		//	}
		//	else {
		//		std::cout << "File not found" << std::endl;
		//	}
		//});


		std::filesystem::path projectLocation = file_dialog({ {{"mrpcp", "Minecraft Resource Pack Creator Project"}} }, false);
		if (!projectLocation.empty()) {
			projectLocation.replace_extension(".mrpcp");

			ref<LoadingPopup> loadingPopup = new LoadingPopup(this, "Creating New Project - " + projectLocation.stem().string());
			auto loadThread = std::thread(&MinecraftResourcePack::newPack, _workingResourcePack, projectLocation, loadingPopup);
			loadThread.detach();
		}


			//namePopup->dispose();
		//});

	//	namePopup->center();
	});

	Button* openButton = new Button(_topMenu, "", FA_FOLDER_OPEN);
	openButton->set_icon_position(Button::IconPosition::LeftCentered);
	openButton->set_fixed_height(this->_topMenuHeight);
	openButton->set_tooltip("Open");
	openButton->set_callback([]() {
		//TODO open
		//std::cout << "Open Result: " << file_dialog({ {"mrpcp", "Minecraft Resource Pack Creator Project"} }, true) << std::endl;
	});

	Button* saveButton = new Button(_topMenu, "", FA_SAVE);
	saveButton->set_icon_position(Button::IconPosition::LeftCentered);
	saveButton->set_fixed_height(this->_topMenuHeight);
	saveButton->set_tooltip("Save");
	saveButton->set_callback([]() {
		//TODO save
		//std::cout << "Save Result: " << file_dialog({ {"mrpcp", "Minecraft Resource Pack Creator Project"} }, true) << std::endl;
	});

	Button* settingsButton = new Button(_topMenu, "", FA_COG);
	settingsButton->set_icon_position(Button::IconPosition::LeftCentered);
	settingsButton->set_fixed_height(this->_topMenuHeight);
	settingsButton->set_tooltip("Settings");
	settingsButton->set_callback([]() {
		//TODO settings
	});



	//Set up the structure browser

	_structureBrowser = new Window(this, "Structure");
	//structureBrowser->set_layout(new GridLayout(Orientation::Vertical, 4));
	_structureBrowser->set_size({ 300,500 });
	_structureBrowser->set_position({ 0, 50 });
	_structureBrowser->set_visible(false);
	auto t = new TreeView(_structureBrowser/*, "a", FA_FILE*/);
	t->set_position({ 0, _structureBrowser->theme()->m_window_header_height });
	t->set_fixed_size({ 300, 500 - _structureBrowser->theme()->m_window_header_height});

	//auto r = new TreeViewItem(t, "a", FA_FILE);//t->getRootItem();
	//new Label(r, "hello");
	//auto q = new TreeViewItem(r, "bbbbbbbbbbbbbbbbbbbbbbbbbbbb", FA_SKULL);
	//new TreeViewItem(r, "b", FA_FILE);


	//Object View
	_objectView = new Window(this, "Object View");
	_objectView->set_size({ 500,500 });
	_objectView->set_position({ 350, 50 });
	_objectView->set_visible(false);
	auto glViewer = new GlViewer(_objectView, { 500, 500 - _objectView->theme()->m_window_header_height });
	glViewer->set_background_color({ 100, 100, 100, 255 });
	glViewer->set_position({ 0, _objectView->theme()->m_window_header_height });


	//Make sure to resize the top menu if the screen is resized
	this->set_resize_callback([this](Vector2i size) {
		this->_topMenu->set_size({ size.x(), this->_topMenuHeight});
	});

	//_workingResourcePack.loadDefault();

	this->perform_layout();
};
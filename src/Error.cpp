#include "Error.h"
#include "Common.h"
#include "nanogui/nanogui.h"
#include "MinecraftResourcePackCreator.h"

using namespace nanogui;

void throwMessage(MessageDialog::Type type, std::string from, std::string message, std::string option0, std::string option1, bool choice, std::function<void(int)> callback) {
	async([type, from, message, callback, option0, option1, choice]() {
		auto dialog = new MessageDialog(Global._app, type, from, message, option0, option1, choice);
		if (callback) {
			dialog->set_callback(callback);
		}
	});
};
void throwMessage(MessageDialog::Type type, std::string from, std::string message, std::function<void(int)> callback) {
	throwMessage(type, from, message, "OK", "", false, callback);
};

void throwWarning(std::string from, std::string message, std::function<void()> callback) {
	std::function<void(int)> wrappedCallback = callback ? [callback](int) {callback(); } : std::function<void(int)>();
	throwMessage(MessageDialog::Type::Warning, "Warning - " + from, message, wrappedCallback);
};
void throwWarningWithChoice(std::string from, std::string message, std::string option0, std::string option1, std::function<void(int)> callback) {
	throwMessage(MessageDialog::Type::Warning, "Warning - " + from, message, option0, option1, true, callback);
};
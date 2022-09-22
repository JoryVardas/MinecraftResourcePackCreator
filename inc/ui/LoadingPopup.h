#pragma once

#include <nanogui/nanogui.h>

class LoadingPopup : public nanogui::Window {
public:
	LoadingPopup(Widget* parent, std::string title, int width = 300);

	const std::string& message() const;
	float progress() const;

	void setMessage(const std::string& message);
	void setProgress(float value);
protected:
	nanogui::ProgressBar* _progressBar;
	nanogui::Label* _message;
};
#pragma once

#include <nanogui/nanogui.h>

class Spacer : public nanogui::Widget {
public:
	Spacer(nanogui::Widget* parent, nanogui::Vector2i size);

	virtual void draw(NVGcontext* ctx) override;
};
#pragma once

#include <nanogui/nanogui.h>

class NonMoveableWindow : public nanogui::Window {
public:
    NonMoveableWindow(nanogui::Widget* parent, const std::string& title = "Untitled");

    virtual bool mouse_drag_event(const nanogui::Vector2i& p, const nanogui::Vector2i& rel, int button, int modifiers) override;
};
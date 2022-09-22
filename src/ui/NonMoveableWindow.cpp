#include "ui/NonMoveableWindow.h"

using namespace nanogui;

NonMoveableWindow::NonMoveableWindow(Widget* parent, const std::string& title) : Window(parent, title) {};

bool NonMoveableWindow::mouse_drag_event(const Vector2i& p, const Vector2i& rel, int button, int modifiers) {
    return false;
};
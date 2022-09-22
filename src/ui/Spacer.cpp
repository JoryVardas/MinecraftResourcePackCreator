#include "ui/Spacer.h"

using namespace nanogui;

Spacer::Spacer(Widget* parent, Vector2i size) : Widget(parent) {
    set_fixed_size(size);
};
void Spacer::draw(NVGcontext* ctx) {
    return;
};
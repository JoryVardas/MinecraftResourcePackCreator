#include "ui/Icon.h"
#include <nanogui/opengl.h>

using namespace nanogui;

Icon::Icon(Widget* parent, int icon) : Widget(parent), _icon(icon) {};

Vector2i Icon::preferred_size(NVGcontext* ctx) const {
    int font_size = m_font_size == -1 ? m_theme->m_button_font_size : m_font_size;
    nvgFontSize(ctx, font_size);
    nvgFontFace(ctx, "sans-bold");
    float iw = 0.0f, ih = font_size;

    if (_icon) {
        if (nvg_is_font_icon(_icon)) {
            ih *= icon_scale();
            nvgFontFace(ctx, "icons");
            nvgFontSize(ctx, ih);
            iw = nvgTextBounds(ctx, 0, 0, utf8(_icon).data(), nullptr, nullptr)
                + m_size.y() * 0.15f;
        }
        else {
            int w, h;
            ih *= 0.9f;
            nvgImageSize(ctx, _icon, &w, &h);
            iw = w * ih / h;
        }
    }
    return Vector2i((int)(iw), font_size);
};

void Icon::draw(NVGcontext* ctx) {
    if (!visible()) return;

    int font_size = m_font_size == -1 ? m_theme->m_button_font_size : m_font_size;
    nvgFontSize(ctx, font_size);
    nvgFontFace(ctx, "sans-bold");

    Vector2f center = Vector2f(m_pos) + Vector2f(m_size.x(), m_size.y()) * 0.5f;
    NVGcolor text_color = m_theme->m_text_color;
    if (!m_enabled)
        text_color = m_theme->m_disabled_text_color;

    if (_icon) {
        auto icon = utf8(_icon);

        float iw, ih = font_size;
        if (nvg_is_font_icon(_icon)) {
            ih *= icon_scale();
            nvgFontSize(ctx, ih);
            nvgFontFace(ctx, "icons");
            iw = nvgTextBounds(ctx, 0, 0, icon.data(), nullptr, nullptr);
        }
        else {
            int w, h;
            ih *= 0.9f;
            nvgImageSize(ctx, _icon, &w, &h);
            iw = w * ih / h;
        }
        nvgFillColor(ctx, text_color);
        nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
        Vector2f icon_pos = { (float)m_pos.x() , center.y() };
        icon_pos.y() -= 1;


        if (nvg_is_font_icon(_icon)) {
            nvgText(ctx, icon_pos.x(), icon_pos.y() + 1, icon.data(), nullptr);
        }
        else {
            NVGpaint img_paint = nvgImagePattern(ctx,
                                                 icon_pos.x(), icon_pos.y() - ih / 2, iw, ih, 0, _icon, m_enabled ? 0.5f : 0.25f);

            nvgFillPaint(ctx, img_paint);
            nvgFill(ctx);
        }
    }
};

void Icon::onMouseButton(const std::function<void(Icon* self, const nanogui::Vector2i& p, int button, bool down, int modifiers)>& callback) {
    _onMouseButton = callback;
};

bool Icon::mouse_button_event(const Vector2i& p, int button, bool down, int modifiers) {
    if (_onMouseButton) _onMouseButton(this, p - m_pos, button, down, modifiers);
    return true;
};

void Icon::setIcon(int icon) {
    _icon = icon;
};
int Icon::icon() const {
    return _icon;
};
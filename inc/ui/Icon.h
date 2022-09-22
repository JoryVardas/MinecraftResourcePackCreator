#pragma once

#include <nanogui/nanogui.h>

class Icon : public nanogui::Widget {
public:
	Icon(nanogui::Widget* parent, int icon);

	virtual nanogui::Vector2i preferred_size(NVGcontext* ctx) const;

	virtual void draw(NVGcontext* ctx);
	
	virtual void onMouseButton(const std::function<void(Icon* self, const nanogui::Vector2i& p, int button, bool down, int modifiers)>& callback);

	virtual bool mouse_button_event(const nanogui::Vector2i& p, int button, bool down, int modifiers);

	virtual void setIcon(int icon);
	virtual int icon() const;

protected:
	int _icon;

	std::function<void(Icon* self, const nanogui::Vector2i& p, int button, bool down, int modifiers)> _onMouseButton;
};
#pragma once

#include <nanogui/nanogui.h>

#include "ui/Spacer.h"
#include "ui/Icon.h"

#include <nanogui/opengl.h>

class TreeView;

class TreeViewItem : public nanogui::Widget {
public:
	TreeViewItem(TreeViewItem* parent, std::string text, int icon, int collapsedIcon = FA_CHEVRON_RIGHT, int expandedIcon = FA_CHEVRON_DOWN);
	TreeViewItem(TreeView* parent, std::string text, int icon, int collapsedIcon = FA_CHEVRON_RIGHT, int expandedIcon = FA_CHEVRON_DOWN);

	virtual bool scroll_event(const nanogui::Vector2i& p, const nanogui::Vector2f& rel);

	const TreeView* getContainingTreeView() const;

	virtual void deselect();
	virtual void select(bool deselectOthers);

	virtual void deselectAll();

	virtual void onSelect(const std::function<void(TreeViewItem*)>& callback);
	virtual void onDeselect(const std::function<void(TreeViewItem*)>& callback);

	//virtual bool mouse_button_event(const nanogui::Vector2i& p, int button, bool down, int modifiers);

	virtual std::string getText() const;

	virtual bool collapsed() const { return _collapsed; };

	virtual void collapse();
	virtual void expand();

	virtual void add_child(int index, nanogui::Widget* widget) override;

protected:
	bool _selected = false;
	bool _collapsed = false;
	bool _isRoot = false;
	int _indent = 0;

	std::function<void(TreeViewItem*)> _onSelectCallback;
	std::function<void(TreeViewItem*)> _onDeselectCallback;

	const int _spacing = 10;
	const int _indentAmount = 20;

private:

	struct TreeViewItemContents : public nanogui::Widget {
		TreeViewItemContents(TreeViewItem* parent, std::string text, int icon, int collapsedIcon, int expandedIcon) : nanogui::Widget(parent), _text(text), _icon(icon), _collapsedIcon(collapsedIcon), _expandedIcon(expandedIcon) {
			set_visible(true);
			set_layout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal, nanogui::Alignment::Fill));

			if (text != "" && icon != 0) {
				new Spacer(this, { parent->_indent, 0 });
				Icon* expandIcon = new Icon(this, _expandedIcon);
				new Icon(this, icon);
				new nanogui::Label(this, text);

				expandIcon->set_visible(false);
			}

			//expandIcon->onMouseButton(expandCollapseClicked);
		};

		virtual TreeViewItem* parent() { return (TreeViewItem*)m_parent; };
		virtual const TreeViewItem* parent() const { return (TreeViewItem*)m_parent; };
		virtual bool mouse_button_event(const nanogui::Vector2i& p, int button, bool down, int modifiers) {
			if (button == GLFW_MOUSE_BUTTON_1 && !down) {
				Icon* expandIcon = dynamic_cast<Icon*>(child_at(1));
				if (expandIcon->visible() && expandIcon->contains(p - m_pos)) {
					if (parent()->collapsed()) {
						parent()->expand();
						expandIcon->setIcon(_expandedIcon);
					}
					else {
						parent()->collapse();
						expandIcon->setIcon(_collapsedIcon);
					}
				}
				else {
					parent()->select(true);
				}

				return true;
			}
		};


		virtual void draw(NVGcontext* ctx);

		virtual bool mouse_enter_event(const nanogui::Vector2i& p, bool enter) override {
			_mouseHover = enter;

			return Widget::mouse_enter_event(p, enter);
		};

		std::string _text;
		int _icon;
		int _expandedIcon;
		int _collapsedIcon;
		bool _mouseHover = false;
	};



	friend TreeView;
};

class TreeView : public nanogui::VScrollPanel {
public:
	TreeView(nanogui::Widget* parent);

	TreeViewItem* getRootItem();

	virtual nanogui::Vector2i preferred_size(NVGcontext* ctx) const override;


	void requireLayoutUpdate() {
		m_update_layout = true;
	};
private:
	friend TreeViewItem;
};
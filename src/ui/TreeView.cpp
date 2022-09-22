#include "ui/TreeView.h"

#include <nanogui/opengl.h>

using namespace nanogui;

TreeViewItem::TreeViewItem(TreeViewItem* parent, std::string text, int icon, int collapsedIcon, int expandedIcon) : Widget(parent) {
    set_visible(true);

    _indent = parent->_indent + _indentAmount;
    
    set_layout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 0, 0));

    new TreeViewItemContents(this, text, icon, collapsedIcon, expandedIcon);
};
TreeViewItem::TreeViewItem(TreeView* parent, std::string text, int icon, int collapsedIcon, int expandedIcon) : Widget(parent), _isRoot(true), _indent(0) {
    set_visible(true);
    set_layout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 0, 0));
    
    new TreeViewItemContents(this, text, icon, collapsedIcon, expandedIcon);
    _indent = -_indentAmount;
};

bool TreeViewItem::scroll_event(const Vector2i& p, const Vector2f& rel) {
    return false;
};



void TreeViewItem::deselect() {
    if (_selected) {
        _selected = false;
        if (_onDeselectCallback) _onDeselectCallback(this);
    }
}
void TreeViewItem::select(bool deselectOthers) {
    if (!_selected) {
        if (deselectOthers) {
            TreeView* treeView = const_cast<TreeView*>(getContainingTreeView());
            treeView->getRootItem()->deselectAll();
        }
        _selected = true;
        if (_onSelectCallback) _onSelectCallback(this);
    }
}
void TreeViewItem::deselectAll() {
    deselect();
    if (child_count() > 0) {
        for (auto it = m_children.begin() + 1; it != m_children.end(); ++it ) {
            dynamic_cast<TreeViewItem*>(*it)->deselectAll();
        }
    }
}

void TreeViewItem::onSelect(const std::function<void(TreeViewItem*)>& callback) {
    _onSelectCallback = callback;
};
void TreeViewItem::onDeselect(const std::function<void(TreeViewItem*)>& callback) {
    _onDeselectCallback = callback;
};


std::string TreeViewItem::getText() const {
    return ((TreeViewItemContents*)m_children[0])->_text;
}




const TreeView* TreeViewItem::getContainingTreeView() const {
    const TreeViewItem* cur = this;
    
    while (!cur->_isRoot) {
        cur = (TreeViewItem*)cur->parent();
    }
    
    return (TreeView*)cur->parent();
};


TreeView::TreeView(Widget* parent) : VScrollPanel(parent) {
    //set_layout(new BoxLayout(Orientation::Vertical, Alignment::Middle, 0, 0));
    new TreeViewItem(this, "", 0, 0, 0);
};

TreeViewItem* TreeView::getRootItem() {
    return (TreeViewItem*)child_at(0);
};

Vector2i TreeView::preferred_size(NVGcontext* ctx) const {
    if (m_size != Vector2i(0, 0)) {
        return m_size;
    }

    if (child_count() != 0)
        return m_children[0]->preferred_size(ctx);
    else
        return { 0, 0 };
}










void TreeViewItem::collapse() {
    _collapsed = true;

    for (auto child : m_children) {
        child->set_visible(false);
    }
    m_children[0]->set_visible(true);

    const_cast<TreeView*>(getContainingTreeView())->m_update_layout = true;
};
void TreeViewItem::expand() {
    _collapsed = false;

    for (auto child : m_children) {
        child->set_visible(true);
    }

    const_cast<TreeView*>(getContainingTreeView())->m_update_layout = true;
};

void TreeViewItem::add_child(int index, Widget* widget) {
    Widget::add_child(index, widget);

    if (child_count() > 1) {
        if (child_at(0)->child_count() > 0) {
            child_at(0)->child_at(1)->set_visible(true);
        }
    }

};



void TreeViewItem::TreeViewItemContents::draw(NVGcontext* ctx) {

    //check if the item contents are even visible in the tree view.
    //this is to prevent a graphical glitch where the nvgIntersectScissor fails to remove the top edges of the items that are past the edge of the tree view.
    const TreeView* treeView = parent()->getContainingTreeView();
    Vector2i treeViewPosition = treeView->absolute_position();
    Vector2i thisPosition = this->absolute_position();

    if(((thisPosition.y() + this->height()) < treeViewPosition.y())
                || (thisPosition.y() > (treeViewPosition.y() + treeView->height())))
        return;

    if (parent()->_selected) {
        nvgBeginPath(ctx);
        nvgRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y());
        nvgFillColor(ctx, m_theme->m_button_gradient_top_pushed);
        //nvgFillColor(ctx, nvgRGBA(255, 0, 0, 255));
        nvgFill(ctx);
    }
    else if (_mouseHover) {
        nvgFillColor(ctx, m_theme->m_button_gradient_top_focused);
    }
    else {
        nvgFillColor(ctx, nvgRGBA(255, 0, 0, 255));
    }


    Widget::draw(ctx);
};
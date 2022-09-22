#include "ui/LoadingPopup.h"

using namespace nanogui;

LoadingPopup::LoadingPopup(Widget* parent, std::string title, int width) : Window(parent, title) {
    set_layout(new BoxLayout(Orientation::Vertical, Alignment::Middle, 10, 10));
    set_modal(true);

    _message = new Label(this, "Your Friendly Loading Screen");
    _message->set_fixed_width(width);

    _progressBar = new ProgressBar(this);
    _progressBar->set_fixed_width(width);
    _progressBar->set_value(0);

    center();
    request_focus();
};



const std::string& LoadingPopup::message() const {
    return _message->caption();
};
float LoadingPopup::progress() const {
    return _progressBar->value();
};

void LoadingPopup::setMessage(const std::string& message) {
    _message->set_caption(message);
};
void LoadingPopup::setProgress(float value) {
    _progressBar->set_value(value);
};
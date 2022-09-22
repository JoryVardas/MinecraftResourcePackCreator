#include <memory>
#include <stdexcept>
#include <iostream>

#include <nanogui/nanogui.h>

#include "Common.h"
#include "MinecraftResourcePackCreator.h"

int main()
{
    try {
        nanogui::init();

        Global._app = new MinecraftResourcePackCreatorApp();
        Global._app->draw_all();
        Global._app->set_visible(true);


        nanogui::mainloop(1 / 60.f * 1000);

        nanogui::shutdown();
    }
    catch (const std::exception& e) {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
        std::cerr << error_msg << std::endl;
        return -1;
    }

    return 0;
}
module;

// исправить в clang++21
#include <spdlog/spdlog.h>
#include <string>
#include <nlohmann/json.hpp>


export module hyprwin;

import comand_exec;

export struct HyprWin {
    std::string win_title;
    std::string win_class;
    int pid;

    void update_info() {
        try {
            auto js = nlohmann::json::parse(execute("hyprctl activewindow -j"));

            win_title = js.value("title", "Empty");
            win_class = js.value("class", "Empty");
            pid = js.value("pid", 0);
        } catch (const std::exception& e) {
            spdlog::error("error in HyprWin::update_info() {}", e.what());
            win_title = "Error";
            win_class = "Error";
            pid = 0;
        }
    }
};
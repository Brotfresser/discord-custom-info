#include <spdlog/spdlog.h>
#include <thread>
#include <chrono>

#include <unistd.h>
#include <csignal>

#include <optional>

import discord_mgr;
import hyprwin;

bool is_running(true);


void signalHandler(int signal) {
    
    spdlog::info("🔻 Received signal: {}", signal);
    spdlog::info("🔄 Performing graceful shutdown...");
    is_running = false;
}

void update_discord_mgr(DiscordManager& mgr, HyprWin& cur_win);


int main() {
    struct sigaction sa;
    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);


    std::optional<DiscordManager> mgr;
    try {
        mgr.emplace();
    }
    catch (std::exception& ex) {
        if (strcmp(ex.what(), discord_mgr_error_init_msg) == 0)
            spdlog::error("Discord is not running");
        else
            spdlog::error("Can't init discord (idk what's the problem) {}", ex.what());

        spdlog::error("terminating program");
        exit(EXIT_FAILURE);
    }
    HyprWin cur_win;


    spdlog::info("all work!");
    while (is_running) {
        cur_win.update_info();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        update_discord_mgr(*mgr, cur_win);

        mgr->runCallbacks();
    }
    spdlog::info("program end");
    
}


void update_discord_mgr(DiscordManager& mgr, HyprWin& cur_win) {
    auto& win_class = cur_win.win_class;
    auto& win_title = cur_win.win_title;

    if (win_class == "Empty")
        mgr.update_presence("Смотрю на рабочий стол, видимо", "default");
    else if (win_class == "code")
        mgr.update_presence("Занят жёстким кодингом: " + win_title.substr(0, win_title.find(" - ")), "vscode");
    else if (win_class == "discord")
        mgr.update_presence("С тобой базарю", "discord");
    else if (win_class == "firefox") 
        if (win_title.find("YouTube") != win_class.npos)
            mgr.update_presence("Смотрю YouTube: " + win_title.substr(0, win_title.find("YouTube")), "youtube");
        else
            mgr.update_presence(win_title, "firefox");
    else
        mgr.update_presence(win_title, "default");
}

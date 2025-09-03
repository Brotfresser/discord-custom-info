module;

// исправить в clang++21
#include <spdlog/spdlog.h>
#include <string>
#include <discord.h>


export module discord_mgr;
export const char *discord_mgr_error_init_msg = "failed to init discord_mgr";

export class DiscordManager {
private:
    discord::Core* core{};
    
public:
    DiscordManager() {
        discord::Result result = discord::Core::Create(1410861882428096523, 
            DiscordCreateFlags_Default | DiscordCreateFlags_NoRequireDiscord,
            &core);
        
        if (result != discord::Result::Ok || !core)
            throw std::runtime_error(discord_mgr_error_init_msg);
    }
    
    bool update_presence(const std::string& detail, const std::string& img) {
        if (!core) return false;

        discord::Activity activity{};
        activity.SetDetails(detail.c_str());
        activity.SetState("Playing Solo");
        activity.GetAssets().SetLargeImage(img.c_str());
        activity.GetAssets().SetLargeText("AAAAAA");
        //activity.SetType(discord::ActivityType::Streaming);
        
        core->ActivityManager().UpdateActivity(activity, 
            [](discord::Result result) {
                if (result != discord::Result::Ok)
                    spdlog::error("failed to update in Presence");
            }
        );

        return true;
    }
    
    void runCallbacks() {
        if (core)
            core->RunCallbacks();
        else
            spdlog::error("ds_mgr core is empty - can't runCallbacks!");
    }

    ~DiscordManager() {
        if (!core)
            return;

        // почему-то сообщения не выводятся
        core->ActivityManager().ClearActivity(
            [](discord::Result result) {
                if (result == discord::Result::Ok)
                    spdlog::info("in dest good clear code");
                else
                    spdlog::error("in dest failed to clear core?");
            }
        );

    }
};
#include "Config.hpp"
#include <hyprland/src/plugins/PluginAPI.hpp>
#include <hyprland/src/config/values/types/StringValue.hpp>
#include <hyprland/src/config/values/types/FloatValue.hpp>

namespace Config {
    std::string imagePath = "";
    float opacity = 0.5f;

    SP<Config::Values::CStringValue> pPath;
    SP<Config::Values::CFloatValue> pOpacity;

    void init() {
        pPath = makeShared<Config::Values::CStringValue>("plugin:watermark:path", "Path to watermark image", "");
        pOpacity = makeShared<Config::Values::CFloatValue>("plugin:watermark:opacity", "Opacity of watermark", 0.5f);

        HyprlandAPI::addConfigValueV2(PHANDLE, pPath);
        HyprlandAPI::addConfigValueV2(PHANDLE, pOpacity);
        reload();
    }

    void reload() {
        if (pPath) imagePath = pPath->value();
        if (pOpacity) opacity = pOpacity->value();
    }
}

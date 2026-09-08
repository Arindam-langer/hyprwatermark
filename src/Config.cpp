#include "Config.hpp"
#include <hyprland/src/plugins/PluginAPI.hpp>
#include <hyprland/src/config/ConfigValue.hpp>

namespace Config {
    std::string imagePath = "";
    float opacity = 0.5f;

    void init() {
        HyprlandAPI::addConfigValue(PHANDLE, "plugin:watermark:path", Hyprlang::STRING{""});
        HyprlandAPI::addConfigValue(PHANDLE, "plugin:watermark:opacity", Hyprlang::FLOAT{0.5f});
        reload();
    }

    void reload() {
        static auto* const PPATH = (Hyprlang::STRING const*)HyprlandAPI::getConfigValue(PHANDLE, "plugin:watermark:path")->getDataStaticPtr();
        static auto* const POPACITY = (Hyprlang::FLOAT const*)HyprlandAPI::getConfigValue(PHANDLE, "plugin:watermark:opacity")->getDataStaticPtr();
        
        imagePath = *PPATH;
        opacity = *POPACITY;
    }
}

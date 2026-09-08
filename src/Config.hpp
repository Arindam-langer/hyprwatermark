#pragma once
#include <string>
#include <hyprland/src/plugins/PluginAPI.hpp>

inline HANDLE PHANDLE = nullptr;

namespace Config {
    extern std::string imagePath;
    extern float opacity;

    void init();
    void reload();
}

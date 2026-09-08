#pragma once

#include <hyprland/src/plugins/PluginAPI.hpp>
#include <string>

inline HANDLE PHANDLE = nullptr;

namespace Config {
extern std::string imagePath;
extern float opacity;
extern float scale;
extern std::string position;
extern float offsetX;
extern float offsetY;

void init();
void reload();
} // namespace Config
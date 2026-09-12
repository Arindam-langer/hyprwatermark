#pragma once

#include <hyprland/src/plugins/PluginAPI.hpp>
#include <string>
#include <vector>

inline HANDLE PHANDLE = nullptr;

namespace Config {
extern std::string imagePath;
extern float opacity;
extern float scale;
extern std::string position;
extern float offsetX;
extern float offsetY;
extern std::vector<std::string> excludePatterns;

void init();
void reload();
bool isExcluded(const std::string& windowClass);
} // namespace Config
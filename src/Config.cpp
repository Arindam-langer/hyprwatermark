#include "Config.hpp"
#include <fnmatch.h>
#include <hyprland/src/config/values/types/FloatValue.hpp>
#include <hyprland/src/config/values/types/StringValue.hpp>
#include <hyprland/src/plugins/PluginAPI.hpp>
#include <sstream>

namespace Config {
std::string imagePath = "";
float opacity = 0.5f;
float scale = 0.35f;
std::string position = "center";
float offsetX = 0.0f;
float offsetY = 0.0f;
std::vector<std::string> excludePatterns;

SP<Config::Values::CStringValue> pPath;
SP<Config::Values::CFloatValue> pOpacity;
SP<Config::Values::CFloatValue> pScale;
SP<Config::Values::CStringValue> pPosition;
SP<Config::Values::CFloatValue> pOffsetX;
SP<Config::Values::CFloatValue> pOffsetY;
SP<Config::Values::CStringValue> pExclude;

static std::string trim(const std::string& s) {
  auto start = s.find_first_not_of(" \t\r\n");
  if (start == std::string::npos)
    return "";
  auto end = s.find_last_not_of(" \t\r\n");
  return s.substr(start, end - start + 1);
}

void init() {
  pPath = makeShared<Config::Values::CStringValue>(
      "plugin:watermark:path", "Path to watermark image", "");

  pOpacity = makeShared<Config::Values::CFloatValue>(
      "plugin:watermark:opacity", "Opacity of watermark", 0.5f);

  pScale = makeShared<Config::Values::CFloatValue>("plugin:watermark:scale",
                                                   "Scale of watermark", 0.35f);

  pPosition = makeShared<Config::Values::CStringValue>(
      "plugin:watermark:position", "Position of watermark", "center");

  pOffsetX = makeShared<Config::Values::CFloatValue>(
      "plugin:watermark:offset_x", "Horizontal watermark offset", 0.0f);

  pOffsetY = makeShared<Config::Values::CFloatValue>(
      "plugin:watermark:offset_y", "Vertical watermark offset", 0.0f);

  pExclude = makeShared<Config::Values::CStringValue>(
      "plugin:watermark:exclude", "Comma-separated list of window classes or patterns to exclude", "");

  HyprlandAPI::addConfigValueV2(PHANDLE, pPath);
  HyprlandAPI::addConfigValueV2(PHANDLE, pOpacity);
  HyprlandAPI::addConfigValueV2(PHANDLE, pScale);
  HyprlandAPI::addConfigValueV2(PHANDLE, pPosition);
  HyprlandAPI::addConfigValueV2(PHANDLE, pOffsetX);
  HyprlandAPI::addConfigValueV2(PHANDLE, pOffsetY);
  HyprlandAPI::addConfigValueV2(PHANDLE, pExclude);

  reload();
}

void reload() {
  if (pPath)
    imagePath = pPath->value();

  if (pOpacity)
    opacity = pOpacity->value();

  if (pScale)
    scale = pScale->value();

  if (pPosition)
    position = pPosition->value();

  if (pOffsetX)
    offsetX = pOffsetX->value();

  if (pOffsetY)
    offsetY = pOffsetY->value();

  excludePatterns.clear();
  if (pExclude) {
    std::string raw = pExclude->value();
    std::stringstream ss(raw);
    std::string item;
    while (std::getline(ss, item, ',')) {
      std::string trimmed = trim(item);
      if (!trimmed.empty())
        excludePatterns.push_back(trimmed);
    }
  }
}

bool isExcluded(const std::string& windowClass) {
  if (excludePatterns.empty())
    return false;

  for (const auto& pattern : excludePatterns) {
    if (fnmatch(pattern.c_str(), windowClass.c_str(), 0) == 0)
      return true;
  }
  return false;
}
} // namespace Config
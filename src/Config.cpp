#include "Config.hpp"
#include <hyprland/src/config/values/types/FloatValue.hpp>
#include <hyprland/src/config/values/types/StringValue.hpp>
#include <hyprland/src/plugins/PluginAPI.hpp>

namespace Config {
std::string imagePath = "";
float opacity = 0.5f;
float scale = 0.35f;
std::string position = "center";
float offsetX = 0.0f;
float offsetY = 0.0f;

SP<Config::Values::CStringValue> pPath;
SP<Config::Values::CFloatValue> pOpacity;
SP<Config::Values::CFloatValue> pScale;
SP<Config::Values::CStringValue> pPosition;
SP<Config::Values::CFloatValue> pOffsetX;
SP<Config::Values::CFloatValue> pOffsetY;
std::vector<std::string> excludeClasses;

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

  HyprlandAPI::addConfigValueV2(PHANDLE, pPath);
  HyprlandAPI::addConfigValueV2(PHANDLE, pOpacity);
  HyprlandAPI::addConfigValueV2(PHANDLE, pScale);
  HyprlandAPI::addConfigValueV2(PHANDLE, pPosition);
  HyprlandAPI::addConfigValueV2(PHANDLE, pOffsetX);
  HyprlandAPI::addConfigValueV2(PHANDLE, pOffsetY);

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
}
} // namespace Config
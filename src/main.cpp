#include "Config.hpp"
#include "TextureManager.hpp"
#include "WatermarkDecoration.hpp"
#include <algorithm>
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/desktop/state/WindowState.hpp>
#include <hyprland/src/event/EventBus.hpp>
#include <hyprland/src/plugins/PluginAPI.hpp>
#include <hyprutils/signal/Listener.hpp>
#include <vector>

std::vector<CWatermarkDecoration*> g_Decorations;
std::vector<Hyprutils::Signal::CHyprSignalListener> g_Listeners;

static void damageAllDecorations() {
  for (auto* d : g_Decorations) {
    if (d)
      d->damageEntire();
  }
}

static void onConfigReloaded() {
  Config::reload();
  TextureManager::loadTexture(Config::imagePath);
  damageAllDecorations();
}

static void onNewWindow(PHLWINDOW window) {
  // prevent duplicates
  if (std::ranges::any_of(window->m_windowDecorations, [](const auto &deco) {
        return deco->getDisplayName() == "HyprWatermark";
      })) {
    return;
  }

  auto deco = makeUnique<CWatermarkDecoration>(window);
  auto* raw = deco.get();
  HyprlandAPI::addWindowDecoration(PHANDLE, window, std::move(deco));
  g_Decorations.push_back(raw);
}

static void onCloseWindow(PHLWINDOW window) {
  std::erase_if(g_Decorations, [&window](CWatermarkDecoration* d) {
    return !d || d->getOwner() == window;
  });
}

APICALL EXPORT std::string PLUGIN_API_VERSION() { return HYPRLAND_API_VERSION; }

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
  PHANDLE = handle;

  Config::init();
  TextureManager::loadTexture(Config::imagePath);

  g_Listeners.push_back(Event::bus()->m_events.window.open.listen(onNewWindow));
  g_Listeners.push_back(
      Event::bus()->m_events.window.close.listen(onCloseWindow));
  g_Listeners.push_back(Event::bus()->m_events.config.reloaded.listen(
      []() { onConfigReloaded(); }));

  // attach to existing windows
  for (auto &w : Desktop::windowState()->windows()) {
    if (w->m_isMapped) {
      onNewWindow(w);
    }
  }

  return {"hypr-watermark", "Per-window background watermark PoC", "aru",
          "0.1.0"};
}

APICALL EXPORT void PLUGIN_EXIT() {
  g_Listeners.clear();
  g_Decorations.clear();
}

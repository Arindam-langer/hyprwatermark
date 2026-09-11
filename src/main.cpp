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
extern "C" {
#include <lauxlib.h>
#include <lua.h>
}

std::vector<CWatermarkDecoration *> g_Decorations;
std::vector<Hyprutils::Signal::CHyprSignalListener> g_Listeners;

static void damageAllDecorations() {
  for (auto *d : g_Decorations) {
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
  auto *raw = deco.get();
  HyprlandAPI::addWindowDecoration(PHANDLE, window, std::move(deco));
  g_Decorations.push_back(raw);
}

static void onCloseWindow(PHLWINDOW window) {
  std::erase_if(g_Decorations, [&window](CWatermarkDecoration *d) {
    return !d || d->getOwner() == window;
  });
}

APICALL EXPORT std::string PLUGIN_API_VERSION() { return HYPRLAND_API_VERSION; }

// exclude list applications for watermark from config
static int luaExclude(lua_State *L) {
  if (!lua_istable(L, 1)) {
    luaL_error(L, "watermark.exclude expects a table");
    return 0;
  }

  Config::excludeClasses.clear();

  const size_t length = lua_rawlen(L, 1);

  for (size_t i = 1; i <= length; ++i) {
    lua_rawgeti(L, 1, i);

    if (!lua_isstring(L, -1)) {
      lua_pop(L, 1);
      luaL_error(L, "watermark.exclude expects a table of strings");
      return 0;
    }

    Config::excludeClasses.emplace_back(lua_tostring(L, -1));

    lua_pop(L, 1);
  }

  damageAllDecorations();

  return 0;
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
  PHANDLE = handle;

  // exluding
  HyprlandAPI::addLuaFunction(PHANDLE, "watermark", "exclude", luaExclude);

  Config::init();
  HyprlandAPI::reloadConfig();
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

  return {"hyprwatermark",
          "Customizable image watermarks inside application windows",
          "Arindam-langer", "0.1.0"};
}

APICALL EXPORT void PLUGIN_EXIT() {
  g_Listeners.clear();
  g_Decorations.clear();
}

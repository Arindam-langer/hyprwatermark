#include "WatermarkDecoration.hpp"
#include "Config.hpp"
#include "TextureManager.hpp"
#include <hyprland/src/plugins/PluginAPI.hpp>
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/desktop/state/WindowState.hpp>
#include <hyprland/src/event/EventBus.hpp>
#include <hyprutils/signal/Listener.hpp>
#include <vector>
#include <memory>
#include <algorithm>

inline HANDLE PHANDLE = nullptr;
std::vector<WP<CWatermarkDecoration>> g_Decorations;
std::vector<Hyprutils::Signal::CHyprSignalListener> g_Listeners;

static void damageAllDecorations() {
    for (auto& deco : g_Decorations) {
        if (auto d = deco.get()) {
            d->damageEntire();
        }
    }
}

static void onConfigReloaded() {
    Config::reload();
    TextureManager::loadTexture(Config::imagePath);
    damageAllDecorations();
}

static void onNewWindow(PHLWINDOW window) {
    // prevent duplicates
    if (std::ranges::any_of(window->m_windowDecorations, [](const auto& deco) {
            return deco->getDisplayName() == "HyprWatermark";
        })) {
        return;
    }

    auto deco = makeUnique<CWatermarkDecoration>(window);
    g_Decorations.emplace_back(deco);
    HyprlandAPI::addWindowDecoration(PHANDLE, window, std::move(deco));
}

static void onCloseWindow(PHLWINDOW window) {
    std::erase_if(g_Decorations, [&window](const auto& deco) {
        auto* d = deco.get();
        return !d || d->getOwner() == window;
    });
}

APICALL EXPORT std::string PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE = handle;

    Config::init();
    TextureManager::loadTexture(Config::imagePath);

    g_Listeners.push_back(
        Event::bus()->m_events.window.open.listen(onNewWindow)
    );
    g_Listeners.push_back(
        Event::bus()->m_events.window.close.listen(onCloseWindow)
    );
    g_Listeners.push_back(
        Event::bus()->m_events.configReloaded.listen([](std::any) { onConfigReloaded(); })
    );

    // attach to existing windows
    for (auto& w : Desktop::windowState()->windows()) {
        if (w->m_isMapped) {
            onNewWindow(w);
        }
    }

    return {"hypr-watermark", "Per-window background watermark PoC", "aru", "0.1.0"};
}

APICALL EXPORT void PLUGIN_EXIT() {
    g_Listeners.clear();
    g_Decorations.clear();
}

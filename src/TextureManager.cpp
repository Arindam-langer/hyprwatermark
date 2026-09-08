#include "TextureManager.hpp"
#include <hyprgraphics/image/Image.hpp>
#include <hyprland/src/render/Renderer.hpp>
#include <hyprland/src/debug/Log.hpp>
#include <filesystem>

namespace TextureManager {
    SP<Render::ITexture> globalTexture = nullptr;

    void loadTexture(const std::string& path) {
        if (path.empty() || !std::filesystem::exists(path)) {
            globalTexture = nullptr;
            return;
        }

        Hyprgraphics::CImage img(path);
        if (!img.loadSuccess) {
            Debug::log(ERR, "[hypr-watermark] Failed to load image: {}", path);
            globalTexture = nullptr;
            return;
        }

        auto cairoSurf = img.cairoSurface();
        if (!cairoSurf || !cairoSurf->cairo()) {
            Debug::log(ERR, "[hypr-watermark] Failed to get cairo surface for: {}", path);
            globalTexture = nullptr;
            return;
        }

        globalTexture = g_pHyprRenderer->createTexture(cairoSurf->cairo());
        Debug::log(LOG, "[hypr-watermark] Successfully loaded texture: {}", path);
    }
}

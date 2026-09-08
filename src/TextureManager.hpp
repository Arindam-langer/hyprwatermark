#pragma once
#include <hyprland/src/render/Texture.hpp>
#include <memory>
#include <string>

namespace TextureManager {
extern SP<Render::ITexture> globalTexture;

void loadTexture(const std::string &path);
} // namespace TextureManager

#include "WatermarkDecoration.hpp"
#include "Config.hpp"
#include "TextureManager.hpp"

#include <algorithm>
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/render/Renderer.hpp>
#include <hyprland/src/render/decorations/IHyprWindowDecoration.hpp>
#include <hyprland/src/render/pass/TexPassElement.hpp>
#include <hyprutils/math/Misc.hpp>

CWatermarkDecoration::CWatermarkDecoration(PHLWINDOW window)
    : IHyprWindowDecoration(window), m_window(window) {}

CWatermarkDecoration::~CWatermarkDecoration() {
  if (m_hasLastBox)
    g_pHyprRenderer->damageBox(m_lastBox);
}

SDecorationPositioningInfo CWatermarkDecoration::getPositioningInfo() {
  SDecorationPositioningInfo info;
  info.policy = DECORATION_POSITION_ABSOLUTE;
  info.edges = 0;
  info.priority = 1;
  info.reserved = false;
  info.desiredExtents = {};
  return info;
}

void CWatermarkDecoration::onPositioningReply(
    const SDecorationPositioningReply & /*reply*/) {}

// window exclude
static bool isExcluded(PHLWINDOW window) {
  if (!window)
    return true;

  return std::ranges::find(Config::excludeClasses, window->m_class) !=
         Config::excludeClasses.end();
}
void CWatermarkDecoration::draw(PHLMONITOR monitor, float const &alpha) {
  auto w = m_window.lock();

  if (!w || !w->m_isMapped || !w->m_workspace || !w->m_workspace->isVisible() ||
      !TextureManager::globalTexture || isExcluded(w))
    return;

  auto pos = w->position(Desktop::View::IGeometric::GEOMETRIC_CURRENT);

  auto size = w->size(Desktop::View::IGeometric::GEOMETRIC_CURRENT);

  const auto textureSize = TextureManager::globalTexture->m_size;

  const float watermarkWidth = textureSize.x * Config::scale;

  const float watermarkHeight = textureSize.y * Config::scale;

  float x = pos.x;
  float y = pos.y;

  if (Config::position == "center") {
    x += (size.x - watermarkWidth) / 2.0f;
    y += (size.y - watermarkHeight) / 2.0f;
  } else if (Config::position == "top-left") {
    // Window origin.
  } else if (Config::position == "top-right") {
    x += size.x - watermarkWidth;
  } else if (Config::position == "bottom-left") {
    y += size.y - watermarkHeight;
  } else if (Config::position == "bottom-right") {
    x += size.x - watermarkWidth;
    y += size.y - watermarkHeight;
  } else {
    // Unknown position: default to center.
    x += (size.x - watermarkWidth) / 2.0f;
    y += (size.y - watermarkHeight) / 2.0f;
  }

  x += Config::offsetX;
  y += Config::offsetY;

  CBox box = {
      x - monitor->position().x,
      y - monitor->position().y,
      watermarkWidth,
      watermarkHeight,
  };

  // Damage the previous watermark area only if its geometry changed.
  if (m_hasLastBox &&
      (m_lastBox.x != box.x || m_lastBox.y != box.y ||
       m_lastBox.width != box.width || m_lastBox.height != box.height)) {
    g_pHyprRenderer->damageBox(m_lastBox);
  }

  m_lastBox = box;
  m_hasLastBox = true;

  CTexPassElement::SRenderData data;
  data.tex = TextureManager::globalTexture;
  data.box = box;
  data.a = Config::opacity * alpha;
  data.round = static_cast<int>(w->rounding());
  data.roundingPower = w->roundingPower();

  g_pHyprRenderer->m_renderPass.add(makeUnique<CTexPassElement>(data));
}

eDecorationType CWatermarkDecoration::getDecorationType() {
  return DECORATION_CUSTOM;
}

void CWatermarkDecoration::updateWindow(PHLWINDOW /*window*/) {
  damageEntire();
}

void CWatermarkDecoration::damageEntire() {
  if (m_hasLastBox)
    g_pHyprRenderer->damageBox(m_lastBox);
}

eDecorationLayer CWatermarkDecoration::getDecorationLayer() {
  return DECORATION_LAYER_OVER;
}

uint64_t CWatermarkDecoration::getDecorationFlags() {
  return DECORATION_NON_SOLID;
}

std::string CWatermarkDecoration::getDisplayName() { return "HyprWatermark"; }

PHLWINDOW CWatermarkDecoration::getOwner() { return m_window.lock(); }
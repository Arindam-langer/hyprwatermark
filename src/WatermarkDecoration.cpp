#include "WatermarkDecoration.hpp"
#include "Config.hpp"
#include "TextureManager.hpp"
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/render/Renderer.hpp>
#include <hyprland/src/render/pass/TexPassElement.hpp>
#include <hyprutils/math/Misc.hpp>

CWatermarkDecoration::CWatermarkDecoration(PHLWINDOW window)
    : IHyprWindowDecoration(window), m_window(window) {}

CWatermarkDecoration::~CWatermarkDecoration() {}

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

void CWatermarkDecoration::draw(PHLMONITOR monitor, float const &alpha) {
  auto w = m_window.lock();
  if (!w || !w->m_isMapped)
    return;

  // Use current geometric box
  auto pos = w->position(Desktop::View::IGeometric::GEOMETRIC_CURRENT);
  auto size = w->size(Desktop::View::IGeometric::GEOMETRIC_CURRENT);

  // Convert to monitor-local logical coordinates
  CBox box = {pos.x - monitor->position().x, pos.y - monitor->position().y,
              size.x, size.y};

  if (TextureManager::globalTexture) {
    CTexPassElement::SRenderData data;
    data.tex = TextureManager::globalTexture;
    data.box = box;
    data.a = Config::opacity * alpha;
    data.round = static_cast<int>(w->rounding());
    data.roundingPower = w->roundingPower();

    g_pHyprRenderer->m_renderPass.add(makeUnique<CTexPassElement>(data));
  }
}

eDecorationType CWatermarkDecoration::getDecorationType() {
  return DECORATION_CUSTOM;
}

void CWatermarkDecoration::updateWindow(PHLWINDOW /*window*/) {
  damageEntire();
}
void CWatermarkDecoration::damageEntire() {
  auto w = m_window.lock();
  if (!w)
    return;
  g_pHyprRenderer->damageWindow(w);
}

eDecorationLayer CWatermarkDecoration::getDecorationLayer() {
  return DECORATION_LAYER_OVER;
}

uint64_t CWatermarkDecoration::getDecorationFlags() {
  return DECORATION_NON_SOLID;
}

std::string CWatermarkDecoration::getDisplayName() { return "HyprWatermark"; }

PHLWINDOW CWatermarkDecoration::getOwner() { return m_window.lock(); }

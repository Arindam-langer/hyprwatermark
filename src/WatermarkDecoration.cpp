#include "WatermarkDecoration.hpp"
#include "TextureManager.hpp"
#include "Config.hpp"
#include <hyprland/src/desktop/rule/windowRule/WindowRuleApplicator.hpp>
#include <hyprland/src/render/pass/TexPassElement.hpp>
#include <hyprland/src/render/Renderer.hpp>
#include <hyprland/src/Compositor.hpp>
#include <hyprutils/math/Misc.hpp>
#include <hyprutils/math/Misc.hpp>

CWatermarkDecoration::CWatermarkDecoration(PHLWINDOW window)
    : IHyprWindowDecoration(window), m_window(window) {
    // Apply noblur to prevent blur:new_optimizations from hiding us when idle
    try {
        if (auto w = m_window.lock(); w && w->m_ruleApplicator) {
            w->m_ruleApplicator->noBlur().set(true, Desktop::Types::PRIORITY_SET_PROP);
            m_noBlurApplied = true;
            damageEntire();
        }
    } catch (...) {}
}

CWatermarkDecoration::~CWatermarkDecoration() {
    if (m_noBlurApplied) {
        try {
            if (auto w = m_window.lock(); w && w->m_ruleApplicator) {
                w->m_ruleApplicator->noBlur().unset(Desktop::Types::PRIORITY_SET_PROP);
                damageEntire();
            }
        } catch (...) {}
    }
}

SDecorationPositioningInfo CWatermarkDecoration::getPositioningInfo() {
    SDecorationPositioningInfo info;
    info.policy         = DECORATION_POSITION_ABSOLUTE;
    info.edges          = 0;
    info.priority       = 1;
    info.reserved       = false;
    info.desiredExtents = {};
    return info;
}

void CWatermarkDecoration::onPositioningReply(const SDecorationPositioningReply& reply) {
    // nothing to do for absolute positioning
}

void CWatermarkDecoration::draw(PHLMONITOR monitor, float const& alpha) {
    auto w = m_window.lock();
    if (!w || !w->m_isMapped)
        return;

    // Use current geometric box
    auto pos = w->position(Desktop::View::IGeometric::GEOMETRIC_CURRENT);
    auto size = w->size(Desktop::View::IGeometric::GEOMETRIC_CURRENT);
    
    // Convert to monitor-local logical coordinates
    CBox box = {pos.x - monitor->position().x, pos.y - monitor->position().y, size.x, size.y};

    if (TextureManager::globalTexture) {
        CTexPassElement::SRenderData data;
        data.tex = TextureManager::globalTexture;
        data.box = box;
        data.a = Config::opacity * alpha;
        data.round = w->rounding();
        data.roundingPower = w->roundingPower();

        g_pHyprRenderer->m_renderPass.add(makeUnique<CTexPassElement>(data));
    }
}

eDecorationType CWatermarkDecoration::getDecorationType() {
    return DECORATION_CUSTOM;
}

void CWatermarkDecoration::updateWindow(PHLWINDOW window) {
    damageEntire();
}

void CWatermarkDecoration::damageEntire() {
    auto w = m_window.lock();
    if (!w)
        return;
    g_pHyprRenderer->damageWindow(w);
}

eDecorationLayer CWatermarkDecoration::getDecorationLayer() {
    return DECORATION_LAYER_BOTTOM;
}

uint64_t CWatermarkDecoration::getDecorationFlags() {
    return DECORATION_NON_SOLID;
}

std::string CWatermarkDecoration::getDisplayName() {
    return "HyprWatermark";
}

PHLWINDOW CWatermarkDecoration::getOwner() {
    return m_window.lock();
}

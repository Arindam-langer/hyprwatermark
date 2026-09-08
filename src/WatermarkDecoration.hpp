#pragma once

#include <hyprland/src/desktop/view/Window.hpp>
#include <hyprland/src/render/decorations/IHyprWindowDecoration.hpp>

class CWatermarkDecoration : public IHyprWindowDecoration {
public:
  explicit CWatermarkDecoration(PHLWINDOW window);
  ~CWatermarkDecoration() override;

  SDecorationPositioningInfo getPositioningInfo() override;
  void onPositioningReply(const SDecorationPositioningReply &reply) override;
  void draw(PHLMONITOR monitor, float const &alpha) override;
  eDecorationType getDecorationType() override;
  void updateWindow(PHLWINDOW window) override;
  void damageEntire() override;
  eDecorationLayer getDecorationLayer() override;
  uint64_t getDecorationFlags() override;
  std::string getDisplayName() override;

  PHLWINDOW getOwner();

private:
  PHLWINDOWREF m_window;
  CBox m_lastBox;
  bool m_hasLastBox = false;
};

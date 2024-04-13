int BrowserNonClientFrameViewAura::NonClientHitTest(const gfx::Point& point) {
  return frame_painter_->NonClientHitTest(this, point);
}

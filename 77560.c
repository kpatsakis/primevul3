gfx::Size BrowserNonClientFrameViewAura::GetMinimumSize() {
  return frame_painter_->GetMinimumSize(this);
}

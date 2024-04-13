gfx::Rect BrowserNonClientFrameViewAura::GetBoundsForClientView() const {
  int top_height = NonClientTopBorderHeight(false);
  return frame_painter_->GetBoundsForClientView(top_height, bounds());
}

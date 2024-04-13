void BrowserNonClientFrameViewAura::Layout() {
  bool maximized_layout =
      frame()->IsMaximized() || !browser_view()->IsBrowserTypeNormal();
  frame_painter_->LayoutHeader(this, maximized_layout);
  if (avatar_button())
    LayoutAvatar();
  BrowserNonClientFrameView::Layout();
}

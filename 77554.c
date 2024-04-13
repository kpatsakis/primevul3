BrowserNonClientFrameViewAura::BrowserNonClientFrameViewAura(
    BrowserFrame* frame, BrowserView* browser_view)
    : BrowserNonClientFrameView(frame, browser_view),
      maximize_button_(NULL),
      close_button_(NULL),
      window_icon_(NULL),
      frame_painter_(new ash::FramePainter) {
}

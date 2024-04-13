BrowserNonClientFrameViewAura::GetThemeFrameOverlayBitmap() const {
  ui::ThemeProvider* tp = GetThemeProvider();
  if (tp->HasCustomImage(IDR_THEME_FRAME_OVERLAY) &&
      browser_view()->IsBrowserTypeNormal() &&
      !browser_view()->IsOffTheRecord()) {
    return tp->GetBitmapNamed(ShouldPaintAsActive() ?
        IDR_THEME_FRAME_OVERLAY : IDR_THEME_FRAME_OVERLAY_INACTIVE);
  }
  return NULL;
}

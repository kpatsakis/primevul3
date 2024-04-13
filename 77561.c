const SkBitmap* BrowserNonClientFrameViewAura::GetThemeFrameBitmap() const {
  bool is_incognito = browser_view()->IsOffTheRecord();
  int resource_id;
  if (browser_view()->IsBrowserTypeNormal()) {
    if (ShouldPaintAsActive()) {
      if (is_incognito) {
        return GetCustomBitmap(IDR_THEME_FRAME_INCOGNITO,
                               IDR_AURA_WINDOW_HEADER_BASE_INCOGNITO_ACTIVE);
      }
      return GetCustomBitmap(IDR_THEME_FRAME,
                             IDR_AURA_WINDOW_HEADER_BASE_ACTIVE);
    }
    if (is_incognito) {
      return GetCustomBitmap(IDR_THEME_FRAME_INCOGNITO_INACTIVE,
                             IDR_AURA_WINDOW_HEADER_BASE_INCOGNITO_INACTIVE);
    }
    return GetCustomBitmap(IDR_THEME_FRAME_INACTIVE,
                           IDR_AURA_WINDOW_HEADER_BASE_INACTIVE);
  }
  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  if (ShouldPaintAsActive()) {
    resource_id = is_incognito ?
        IDR_AURA_WINDOW_HEADER_BASE_INCOGNITO_ACTIVE :
        IDR_AURA_WINDOW_HEADER_BASE_ACTIVE;
  } else {
    resource_id = is_incognito ?
        IDR_AURA_WINDOW_HEADER_BASE_INCOGNITO_INACTIVE :
        IDR_AURA_WINDOW_HEADER_BASE_INACTIVE;
  }
  return rb.GetBitmapNamed(resource_id);
}

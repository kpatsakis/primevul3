SkBitmap* BrowserNonClientFrameViewAura::GetCustomBitmap(
    int bitmap_id,
    int fallback_bitmap_id) const {
  ui::ThemeProvider* tp = GetThemeProvider();
  if (tp->HasCustomImage(bitmap_id))
    return tp->GetBitmapNamed(bitmap_id);
  return tp->GetBitmapNamed(fallback_bitmap_id);
}

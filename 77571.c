void BrowserNonClientFrameViewAura::PaintToolbarBackground(
    gfx::Canvas* canvas) {
  gfx::Rect toolbar_bounds(browser_view()->GetToolbarBounds());
  if (toolbar_bounds.IsEmpty())
    return;
  gfx::Point toolbar_origin(toolbar_bounds.origin());
  ConvertPointToView(browser_view(), this, &toolbar_origin);
  toolbar_bounds.set_origin(toolbar_origin);

  int x = toolbar_bounds.x();
  int w = toolbar_bounds.width();
  int y = toolbar_bounds.y();
  int h = toolbar_bounds.height();

  int split_point = kFrameShadowThickness * 2;
  int bottom_y = y + split_point;
  ui::ThemeProvider* tp = GetThemeProvider();
  SkBitmap* theme_toolbar = tp->GetBitmapNamed(IDR_THEME_TOOLBAR);
  int bottom_edge_height = std::min(theme_toolbar->height(), h) - split_point;

  canvas->FillRect(gfx::Rect(x, bottom_y, w, bottom_edge_height),
                   tp->GetColor(ThemeService::COLOR_TOOLBAR));

  bool restored = !frame()->IsMaximized();
  canvas->TileImageInt(
      *theme_toolbar,
      x, bottom_y - GetHorizontalTabStripVerticalOffset(restored),
      x, bottom_y,
      w, theme_toolbar->height());

  SkBitmap* toolbar_center =
      tp->GetBitmapNamed(IDR_CONTENT_TOP_CENTER);
  canvas->TileImageInt(*toolbar_center,
                       0, 0,
                       x, y,
                       w, split_point);

  canvas->FillRect(gfx::Rect(x + kClientEdgeThickness,
                             toolbar_bounds.bottom() - kClientEdgeThickness,
                             w - (2 * kClientEdgeThickness),
                             kClientEdgeThickness),
      ThemeService::GetDefaultColor(ThemeService::COLOR_TOOLBAR_SEPARATOR));
}

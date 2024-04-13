SkBitmap BrowserNonClientFrameViewAura::GetFaviconForTabIconView() {
  views::WidgetDelegate* delegate = frame()->widget_delegate();
  if (!delegate)
    return SkBitmap();
  return delegate->GetWindowIcon();
}

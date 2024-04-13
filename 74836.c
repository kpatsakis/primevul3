void RenderView::OnSetRendererPrefs(const RendererPreferences& renderer_prefs) {
  renderer_preferences_ = renderer_prefs;
  UpdateFontRenderingFromRendererPrefs();
#if defined(TOOLKIT_GTK)
  WebColorName name = WebKit::WebColorWebkitFocusRingColor;
  WebKit::setNamedColors(&name, &renderer_prefs.focus_ring_color, 1);
  WebKit::setCaretBlinkInterval(renderer_prefs.caret_blink_interval);

  if (webview()) {
    webview()->setScrollbarColors(
        renderer_prefs.thumb_inactive_color,
        renderer_prefs.thumb_active_color,
        renderer_prefs.track_color);
    webview()->setSelectionColors(
        renderer_prefs.active_selection_bg_color,
        renderer_prefs.active_selection_fg_color,
        renderer_prefs.inactive_selection_bg_color,
        renderer_prefs.inactive_selection_fg_color);
    didInvalidateRect(gfx::Rect(size_));
  }
#endif
}

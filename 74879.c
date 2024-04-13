bool RenderView::allowScript(WebFrame* frame, bool enabled_per_settings) {
  if (enabled_per_settings &&
      AllowContentType(CONTENT_SETTINGS_TYPE_JAVASCRIPT))
    return true;

  if (IsWhitelistedForContentSettings(frame))
    return true;

  return false;  // Other protocols fall through here.
}

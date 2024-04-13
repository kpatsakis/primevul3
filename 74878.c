bool RenderView::allowImages(WebFrame* frame, bool enabled_per_settings) {
  if (enabled_per_settings &&
      AllowContentType(CONTENT_SETTINGS_TYPE_IMAGES))
    return true;

  if (IsWhitelistedForContentSettings(frame))
    return true;

  DidBlockContentType(CONTENT_SETTINGS_TYPE_IMAGES);
  return false;  // Other protocols fall through here.
}

void RenderView::didNotAllowPlugins(WebKit::WebFrame* frame) {
  DidBlockContentType(CONTENT_SETTINGS_TYPE_PLUGINS);
}

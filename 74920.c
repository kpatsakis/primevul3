void RenderView::didNotAllowScript(WebKit::WebFrame* frame) {
  DidBlockContentType(CONTENT_SETTINGS_TYPE_JAVASCRIPT);
}

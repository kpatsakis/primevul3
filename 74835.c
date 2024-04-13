void RenderView::OnSetPageEncoding(const std::string& encoding_name) {
  webview()->setPageEncoding(WebString::fromUTF8(encoding_name));
}

void RenderFrameImpl::OnCSSInsertRequest(const std::string& css) {
  frame_->document().insertStyleSheet(WebString::fromUTF8(css));
}

const WebKit::WebNode& PrintWebViewHelper::PrintPreviewContext::node() const {
  DCHECK(state_ != UNINITIALIZED);
  return node_;
}

WebElement RenderFrameImpl::GetFocusedElement() {
  WebDocument doc = frame_->document();
  if (!doc.isNull())
    return doc.focusedElement();

  return WebElement();
}

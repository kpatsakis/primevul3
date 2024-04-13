bool WebContentsImpl::IsHidden() {
  return capturer_count_ == 0 && !should_normally_be_visible_;
}

bool WebContentsImpl::IsFullscreenForCurrentTab() const {
  return delegate_ ? delegate_->IsFullscreenForTabOrPending(this) : false;
}

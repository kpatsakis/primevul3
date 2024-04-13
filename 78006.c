void WebContentsImpl::Deactivate() {
  if (delegate_)
    delegate_->DeactivateContents(this);
}

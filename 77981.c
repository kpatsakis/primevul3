void WebContentsImpl::Activate() {
  if (delegate_)
    delegate_->ActivateContents(this);
}

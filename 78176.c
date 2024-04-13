void WebContentsImpl::SetFocusToLocationBar(bool select_all) {
  if (delegate_)
    delegate_->SetFocusToLocationBar(select_all);
}

void WebContentsImpl::NotifyNavigationStateChanged(unsigned changed_flags) {
  if (delegate_)
    delegate_->NavigationStateChanged(this, changed_flags);
}

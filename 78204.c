void WebContentsImpl::ViewSource() {
  if (!delegate_)
    return;

  NavigationEntry* active_entry = GetController().GetActiveEntry();
  if (!active_entry)
    return;

  delegate_->ViewSourceForTab(this, active_entry->GetURL());
}

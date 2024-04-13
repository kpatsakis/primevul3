bool WebContentsImpl::FocusLocationBarByDefault() {
  NavigationEntry* entry = controller_.GetActiveEntry();
  if (entry && entry->GetURL() == GURL(kAboutBlankURL))
    return true;
  return delegate_ && delegate_->ShouldFocusLocationBarByDefault(this);
}

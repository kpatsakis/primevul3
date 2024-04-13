void WebContentsImpl::UpdatePreferredSize(const gfx::Size& pref_size) {
  preferred_size_ = pref_size;
  if (delegate_)
    delegate_->UpdatePreferredSize(this, pref_size);
}

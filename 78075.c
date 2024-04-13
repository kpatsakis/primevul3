gfx::Rect WebContentsImpl::GetRootWindowResizerRect() const {
  if (delegate_)
    return delegate_->GetRootWindowResizerRect();
  return gfx::Rect();
}

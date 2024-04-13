gfx::Size WebContentsImpl::GetSizeForNewRenderView() const {
  gfx::Size size;
  if (delegate_)
    size = delegate_->GetSizeForNewRenderView(this);
  if (size.IsEmpty())
    size = view_->GetContainerSize();
  return size;
}

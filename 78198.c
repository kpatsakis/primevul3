void WebContentsImpl::UpdateRenderViewSizeForRenderManager() {
  gfx::Size size = GetSizeForNewRenderView();
  if (!size.IsEmpty())
    view_->SizeContents(size);
}

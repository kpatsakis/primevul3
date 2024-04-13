void WebContentsImpl::Close(RenderViewHost* rvh) {
#if defined(OS_MACOSX)
  if (view_->IsEventTracking()) {
    view_->CloseTabAfterEventTracking();
    return;
  }
#endif

  if (delegate_ && rvh == GetRenderViewHost())
    delegate_->CloseContents(this);
}

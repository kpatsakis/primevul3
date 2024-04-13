void WebContentsImpl::WasShown() {
  controller_.SetActive(true);
  RenderWidgetHostViewPort* rwhv =
      RenderWidgetHostViewPort::FromRWHV(GetRenderWidgetHostView());
  if (rwhv) {
    rwhv->WasShown();
#if defined(OS_MACOSX)
    rwhv->SetActive(true);
#endif
  }

  last_selected_time_ = base::TimeTicks::Now();

  FOR_EACH_OBSERVER(WebContentsObserver, observers_, WasShown());

  RenderViewHostImpl* rvh =
      static_cast<RenderViewHostImpl*>(GetRenderViewHost());
  if (rvh) {
    rvh->ResizeRectChanged(GetRootWindowResizerRect());
  }

  should_normally_be_visible_ = true;
  NotificationService::current()->Notify(
      NOTIFICATION_WEB_CONTENTS_VISIBILITY_CHANGED,
      Source<WebContents>(this),
      Details<const bool>(&should_normally_be_visible_));
}

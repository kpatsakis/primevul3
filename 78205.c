void WebContentsImpl::WasHidden() {
  if (capturer_count_ == 0) {
    RenderWidgetHostViewPort* rwhv =
        RenderWidgetHostViewPort::FromRWHV(GetRenderWidgetHostView());
    if (rwhv)
      rwhv->WasHidden();
  }

  should_normally_be_visible_ = false;
  NotificationService::current()->Notify(
      NOTIFICATION_WEB_CONTENTS_VISIBILITY_CHANGED,
      Source<WebContents>(this),
      Details<const bool>(&should_normally_be_visible_));
}

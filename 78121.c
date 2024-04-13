void WebContentsImpl::NotifySwapped(RenderViewHost* old_render_view_host) {
  notify_disconnection_ = true;
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    RenderViewHostSwapped(old_render_view_host));

  NotificationService::current()->Notify(
      NOTIFICATION_WEB_CONTENTS_SWAPPED,
      Source<WebContents>(this),
      Details<RenderViewHost>(old_render_view_host));

  RemoveBrowserPluginEmbedder();
}

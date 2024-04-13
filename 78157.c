void WebContentsImpl::RenderViewCreated(RenderViewHost* render_view_host) {
  if (static_cast<RenderViewHostImpl*>(render_view_host)->is_swapped_out())
    return;

  if (delegate_)
    view_->SetOverscrollControllerEnabled(delegate_->CanOverscrollContent());

  NotificationService::current()->Notify(
      NOTIFICATION_WEB_CONTENTS_RENDER_VIEW_HOST_CREATED,
      Source<WebContents>(this),
      Details<RenderViewHost>(render_view_host));

  if (render_manager_.pending_web_ui())
    render_manager_.pending_web_ui()->RenderViewCreated(render_view_host);

  NavigationEntry* entry = controller_.GetActiveEntry();
  if (entry && entry->IsViewSourceMode()) {
    render_view_host->Send(
        new ViewMsg_EnableViewSourceMode(render_view_host->GetRoutingID()));
  }

  view_->RenderViewCreated(render_view_host);

  FOR_EACH_OBSERVER(
      WebContentsObserver, observers_, RenderViewCreated(render_view_host));
}

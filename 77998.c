int WebContentsImpl::CreateOpenerRenderViews(SiteInstance* instance) {
  int opener_route_id = MSG_ROUTING_NONE;

  if (opener_)
    opener_route_id = opener_->CreateOpenerRenderViews(instance);

  if (render_manager_.current_host()->GetSiteInstance() == instance)
    return render_manager_.current_host()->GetRoutingID();

  if (render_manager_.pending_render_view_host() &&
      render_manager_.pending_render_view_host()->GetSiteInstance() == instance)
    return render_manager_.pending_render_view_host()->GetRoutingID();

  RenderViewHostImpl* rvh = render_manager_.GetSwappedOutRenderViewHost(
      instance);
  if (rvh)
    return rvh->GetRoutingID();

  return render_manager_.CreateRenderView(instance, opener_route_id,
                                          true, true);
}

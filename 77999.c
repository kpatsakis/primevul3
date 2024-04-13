bool WebContentsImpl::CreateRenderViewForRenderManager(
    RenderViewHost* render_view_host, int opener_route_id) {
  TRACE_EVENT0("browser", "WebContentsImpl::CreateRenderViewForRenderManager");
  RenderWidgetHostView* rwh_view = view_->CreateViewForWidget(render_view_host);

  if (rwh_view)
    rwh_view->SetSize(GetSizeForNewRenderView());

  UpdateMaxPageIDIfNecessary(render_view_host);
  int32 max_page_id =
      GetMaxPageIDForSiteInstance(render_view_host->GetSiteInstance());

  if (!static_cast<RenderViewHostImpl*>(
          render_view_host)->CreateRenderView(string16(),
                                              opener_route_id,
                                              max_page_id)) {
    return false;
  }

#if defined(OS_POSIX) && !defined(OS_MACOSX) && !defined(OS_ANDROID)
  if (rwh_view) {
    if (RenderWidgetHost* render_widget_host = rwh_view->GetRenderWidgetHost())
      render_widget_host->WasResized();
  }
#endif

  return true;
}

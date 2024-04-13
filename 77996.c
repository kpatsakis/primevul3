void WebContentsImpl::CreateNewWidget(int route_id,
                                      bool is_fullscreen,
                                      WebKit::WebPopupType popup_type) {
  RenderProcessHost* process = GetRenderProcessHost();
  RenderWidgetHostImpl* widget_host =
      new RenderWidgetHostImpl(this, process, route_id, IsHidden());
  created_widgets_.insert(widget_host);

  RenderWidgetHostViewPort* widget_view = RenderWidgetHostViewPort::FromRWHV(
      view_->CreateViewForPopupWidget(widget_host));
  if (!widget_view)
    return;
  if (!is_fullscreen) {
    widget_view->SetPopupType(popup_type);
  }
  pending_widget_views_[route_id] = widget_view;

#if defined(OS_MACOSX)
  base::mac::NSObjectRetain(widget_view->GetNativeView());
#endif
}

void WebContentsImpl::ShowCreatedWidget(int route_id,
                                        bool is_fullscreen,
                                        const gfx::Rect& initial_pos) {
  if (delegate_)
    delegate_->RenderWidgetShowing();

  RenderWidgetHostViewPort* widget_host_view =
      RenderWidgetHostViewPort::FromRWHV(GetCreatedWidget(route_id));
  if (!widget_host_view)
    return;
  bool allow_privileged = false;
  if (is_fullscreen) {
    if (delegate_ && delegate_->EmbedsFullscreenWidget()) {
      widget_host_view->InitAsChild(GetRenderWidgetHostView()->GetNativeView());
      delegate_->ToggleFullscreenModeForTab(this, true);
    } else {
      widget_host_view->InitAsFullscreen(GetRenderWidgetHostViewPort());
      allow_privileged = true;
    }

    DCHECK_EQ(MSG_ROUTING_NONE, fullscreen_widget_routing_id_);
    fullscreen_widget_routing_id_ = route_id;
    FOR_EACH_OBSERVER(WebContentsObserver,
                      observers_,
                      DidShowFullscreenWidget(route_id));
    if (!widget_host_view->HasFocus())
      widget_host_view->Focus();
  } else {
    widget_host_view->InitAsPopup(GetRenderWidgetHostViewPort(), initial_pos);
  }

  RenderWidgetHostImpl* render_widget_host_impl =
      RenderWidgetHostImpl::From(widget_host_view->GetRenderWidgetHost());
  render_widget_host_impl->Init();
  render_widget_host_impl->set_allow_privileged_mouse_lock(allow_privileged);

#if defined(OS_MACOSX)
  base::mac::NSObjectRelease(widget_host_view->GetNativeView());
#endif
}

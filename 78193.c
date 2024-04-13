void WebContentsImpl::ToggleFullscreenMode(bool enter_fullscreen) {
  RenderWidgetHostView* const widget_view = GetFullscreenRenderWidgetHostView();
  if (widget_view)
    RenderWidgetHostImpl::From(widget_view->GetRenderWidgetHost())->Shutdown();

  if (delegate_)
    delegate_->ToggleFullscreenModeForTab(this, enter_fullscreen);
}

RenderWidgetHostView* WebContentsImpl::GetFullscreenRenderWidgetHostView()
    const {
  RenderWidgetHost* const widget_host =
      RenderWidgetHostImpl::FromID(GetRenderProcessHost()->GetID(),
                                   GetFullscreenWidgetRoutingID());
  return widget_host ? widget_host->GetView() : NULL;
}

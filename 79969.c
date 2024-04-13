RenderViewImpl* RenderViewImpl::Create(
    int32 opener_id,
    const RendererPreferences& renderer_prefs,
    const WebPreferences& webkit_prefs,
    SharedRenderViewCounter* counter,
    int32 routing_id,
    int32 surface_id,
    int64 session_storage_namespace_id,
    const string16& frame_name,
    bool is_renderer_created,
    bool swapped_out,
    int32 next_page_id,
    const WebKit::WebScreenInfo& screen_info,
    AccessibilityMode accessibility_mode) {
  DCHECK(routing_id != MSG_ROUTING_NONE);
  RenderViewImplParams params(
      opener_id,
      renderer_prefs,
      webkit_prefs,
      counter,
      routing_id,
      surface_id,
      session_storage_namespace_id,
      frame_name,
      is_renderer_created,
      swapped_out,
      next_page_id,
      screen_info,
      accessibility_mode);
  RenderViewImpl* render_view = NULL;
  if (g_create_render_view_impl)
    render_view = g_create_render_view_impl(&params);
  else
    render_view = new RenderViewImpl(&params);
  render_view->Initialize(&params);
  return render_view;
}

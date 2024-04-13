RenderView* RenderView::Create(
    RenderThreadBase* render_thread,
    gfx::NativeViewId parent_hwnd,
    int32 opener_id,
    const RendererPreferences& renderer_prefs,
    const WebPreferences& webkit_prefs,
    SharedRenderViewCounter* counter,
    int32 routing_id,
    int64 session_storage_namespace_id,
    const string16& frame_name) {
  DCHECK(routing_id != MSG_ROUTING_NONE);
  scoped_refptr<RenderView> view = new RenderView(render_thread, webkit_prefs,
                                                  session_storage_namespace_id);
  view->Init(parent_hwnd,
             opener_id,
             renderer_prefs,
             counter,
             routing_id,
             frame_name);  // adds reference
  return view;
}

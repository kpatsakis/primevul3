gfx::PluginWindowHandle RenderView::AllocateFakePluginWindowHandle(
    bool opaque, bool root) {
  gfx::PluginWindowHandle window = NULL;
  Send(new ViewHostMsg_AllocateFakePluginWindowHandle(
      routing_id(), opaque, root, &window));
  return window;
}

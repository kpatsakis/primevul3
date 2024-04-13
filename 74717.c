void RenderView::AcceleratedSurfaceBuffersSwapped(
    gfx::PluginWindowHandle window) {
  Send(new ViewHostMsg_AcceleratedSurfaceBuffersSwapped(routing_id(), window));
}

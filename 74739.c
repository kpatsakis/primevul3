void RenderView::DestroyFakePluginWindowHandle(gfx::PluginWindowHandle window) {
  if (window)
    Send(new ViewHostMsg_DestroyFakePluginWindowHandle(routing_id(), window));
}

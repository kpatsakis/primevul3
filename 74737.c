void RenderView::CreatedPluginWindow(gfx::PluginWindowHandle window) {
#if defined(USE_X11)
  RenderThread::current()->Send(new ViewHostMsg_CreatePluginContainer(
      routing_id(), window));
#endif
}

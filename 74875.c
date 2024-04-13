void RenderView::WillDestroyPluginWindow(gfx::PluginWindowHandle window) {
#if defined(USE_X11)
  RenderThread::current()->Send(new ViewHostMsg_DestroyPluginContainer(
      routing_id(), window));
#endif
  CleanupWindowInPluginMoves(window);
}

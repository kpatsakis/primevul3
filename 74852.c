void RenderView::OnWindowFrameChanged(const gfx::Rect& window_frame,
                                      const gfx::Rect& view_frame) {
  std::set<WebPluginDelegateProxy*>::iterator plugin_it;
  for (plugin_it = plugin_delegates_.begin();
       plugin_it != plugin_delegates_.end(); ++plugin_it) {
    (*plugin_it)->WindowFrameChanged(window_frame, view_frame);
  }
}

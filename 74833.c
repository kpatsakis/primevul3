void RenderView::OnSetFocus(bool enable) {
  RenderWidget::OnSetFocus(enable);

  if (webview() && webview()->isActive()) {
    std::set<WebPluginDelegateProxy*>::iterator plugin_it;
    for (plugin_it = plugin_delegates_.begin();
         plugin_it != plugin_delegates_.end(); ++plugin_it) {
      if (enable)
        (*plugin_it)->SetWindowFocus(true);
      (*plugin_it)->SetContentAreaFocus(enable);
    }
  }
}

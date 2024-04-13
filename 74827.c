void RenderView::OnSetActive(bool active) {
  if (webview())
    webview()->setIsActive(active);

#if defined(OS_MACOSX)
  std::set<WebPluginDelegateProxy*>::iterator plugin_it;
  for (plugin_it = plugin_delegates_.begin();
       plugin_it != plugin_delegates_.end(); ++plugin_it) {
    (*plugin_it)->SetWindowFocus(active);
  }
#endif
}

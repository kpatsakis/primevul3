DevToolsWindow* DevToolsWindow::AsDevToolsWindow(
    content::RenderViewHost* window_rvh) {
  if (g_instances == NULL)
    return NULL;
  DevToolsWindows* instances = &g_instances.Get();
  for (DevToolsWindows::iterator it(instances->begin()); it != instances->end();
       ++it) {
    if ((*it)->web_contents_->GetRenderViewHost() == window_rvh)
      return *it;
  }
  return NULL;
}

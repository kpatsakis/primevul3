bool DevToolsWindow::IsDevToolsWindow(content::RenderViewHost* window_rvh) {
  return AsDevToolsWindow(window_rvh) != NULL;
}

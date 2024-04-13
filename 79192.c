DevToolsWindow* DevToolsWindow::OpenDevToolsWindow(
    content::RenderViewHost* inspected_rvh) {
  return ToggleDevToolsWindow(
      inspected_rvh, true, DevToolsToggleAction::Show());
}

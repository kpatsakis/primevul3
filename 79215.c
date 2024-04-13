void DevToolsWindow::UpdateBrowserToolbar() {
  BrowserWindow* inspected_window = GetInspectedBrowserWindow();
  if (inspected_window)
    inspected_window->UpdateToolbar(NULL);
}

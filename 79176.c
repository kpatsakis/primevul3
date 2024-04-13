void DevToolsWindow::HandleKeyboardEvent(
    content::WebContents* source,
    const content::NativeWebKeyboardEvent& event) {
  if (IsDocked()) {
    if (event.windowsKeyCode == 0x08) {
      return;
    }
    BrowserWindow* inspected_window = GetInspectedBrowserWindow();
    if (inspected_window)
      inspected_window->HandleKeyboardEvent(event);
  }
}

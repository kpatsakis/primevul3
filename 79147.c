void DevToolsWindow::CloseContents(content::WebContents* source) {
  CHECK(IsDocked());
  BrowserWindow* inspected_window = GetInspectedBrowserWindow();
  if (inspected_window)
    inspected_window->UpdateDevTools();
  delete web_contents_;
}

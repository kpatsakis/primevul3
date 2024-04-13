void DevToolsWindow::OnPageCloseCanceled(content::WebContents* contents) {
  DevToolsWindow *window =
      DevToolsWindow::GetInstanceForInspectedRenderViewHost(
          contents->GetRenderViewHost());
  if (!window)
    return;
  window->intercepted_page_beforeunload_ = false;
  DevToolsWindow::OnPageCloseCanceled(window->web_contents());
}

bool DevToolsWindow::NeedsToInterceptBeforeUnload(
    content::WebContents* contents) {
  DevToolsWindow* window =
      DevToolsWindow::GetInstanceForInspectedRenderViewHost(
          contents->GetRenderViewHost());
  return window && !window->intercepted_page_beforeunload_;
}

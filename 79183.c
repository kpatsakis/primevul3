bool DevToolsWindow::InterceptPageBeforeUnload(content::WebContents* contents) {
  DevToolsWindow* window =
      DevToolsWindow::GetInstanceForInspectedRenderViewHost(
          contents->GetRenderViewHost());
  if (!window || window->intercepted_page_beforeunload_)
    return false;

  window->intercepted_page_beforeunload_ = true;
  if (!DevToolsWindow::InterceptPageBeforeUnload(window->web_contents())) {
    window->web_contents()->GetRenderViewHost()->FirePageBeforeUnload(false);
  }
  return true;
}

bool DevToolsWindow::HandleBeforeUnload(content::WebContents* frontend_contents,
    bool proceed, bool* proceed_to_fire_unload) {
  DevToolsWindow* window = AsDevToolsWindow(
      frontend_contents->GetRenderViewHost());
  DCHECK(window);
  if (!window->intercepted_page_beforeunload_)
    return false;
  window->BeforeUnloadFired(frontend_contents, proceed,
      proceed_to_fire_unload);
  return true;
}

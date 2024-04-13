DevToolsWindow* DevToolsWindow::GetDockedInstanceForInspectedTab(
    content::WebContents* inspected_web_contents) {
  DevToolsWindow* window = GetInstanceForInspectedRenderViewHost(
      inspected_web_contents->GetRenderViewHost());
  return (window && window->IsDocked()) ? window : NULL;
}

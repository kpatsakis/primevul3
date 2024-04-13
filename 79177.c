bool DevToolsWindow::HasFiredBeforeUnloadEventForDevToolsBrowser(
    Browser* browser) {
  DCHECK(browser->is_devtools());
  if (browser->tab_strip_model()->empty())
    return true;
  content::WebContents* contents =
      browser->tab_strip_model()->GetWebContentsAt(0);
  DevToolsWindow* window = AsDevToolsWindow(contents->GetRenderViewHost());
  DCHECK(window);
  return window->intercepted_page_beforeunload_;
}

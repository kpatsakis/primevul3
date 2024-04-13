void DevToolsWindow::FrontendWebContentsObserver::WebContentsDestroyed(
    content::WebContents* contents) {
  delete devtools_window_;
}

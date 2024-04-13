DevToolsWindow::FrontendWebContentsObserver::FrontendWebContentsObserver(
    DevToolsWindow* devtools_window)
    : WebContentsObserver(devtools_window->web_contents()),
      devtools_window_(devtools_window) {
}

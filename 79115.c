DevToolsWindowBeforeUnloadObserver::DevToolsWindowBeforeUnloadObserver(
    DevToolsWindow* devtools_window)
    : WebContentsObserver(devtools_window->web_contents()),
      m_fired(false) {
}

  DevToolsWindow* OpenDevToolWindowOnWebContents(
      content::WebContents* contents) {
    content::WindowedNotificationObserver observer(
        content::NOTIFICATION_LOAD_STOP,
        content::NotificationService::AllSources());
    DevToolsWindow* window = DevToolsWindow::OpenDevToolsWindow(
        contents->GetRenderViewHost());
    observer.Wait();
    return window;
  }

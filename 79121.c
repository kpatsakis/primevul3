  void OpenDevToolsWindow(const std::string& test_page) {
    ASSERT_TRUE(test_server()->Start());
    LoadTestPage(test_page);

    content::WindowedNotificationObserver observer(
        content::NOTIFICATION_LOAD_STOP,
        content::NotificationService::AllSources());
    inspected_rvh_ = GetInspectedTab()->GetRenderViewHost();
    window_ = DevToolsWindow::OpenDevToolsWindow(inspected_rvh_);
    observer.Wait();
  }

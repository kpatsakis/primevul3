  void LoadTestPage(const std::string& test_page) {
    content::WindowedNotificationObserver load_observer(
        content::NOTIFICATION_LOAD_STOP,
        content::NotificationService::AllSources());
    GURL url = test_server()->GetURL(test_page);
    ui_test_utils::NavigateToURL(browser(), url);
    load_observer.Wait();
  }

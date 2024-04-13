  void CloseDevToolsWindow() {
    Browser* browser = window_->browser();
    browser->tab_strip_model()->CloseAllTabs();
    BrowserClosedObserver close_observer(browser);
  }

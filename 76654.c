  virtual void TearDown() {
    test_browser_client_.ClearSchemes();
    GetContentClient()->set_browser_for_testing(old_browser_client_);
  }

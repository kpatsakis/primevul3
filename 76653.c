  virtual void SetUp() {
    old_browser_client_ = GetContentClient()->browser();
    GetContentClient()->set_browser_for_testing(&test_browser_client_);

    test_browser_client_.AddScheme("chrome");
  }

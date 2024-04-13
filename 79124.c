  void RunTest(const std::string& test_name, const std::string& test_page) {
    OpenDevToolsWindow(test_page);
    RunTestFunction(window_, test_name.c_str());
    CloseDevToolsWindow();
  }

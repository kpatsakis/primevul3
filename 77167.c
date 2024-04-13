  void RunTest(const FilePath& url,
               const char* expected_reply,
               bool new_tab) {
    FilePath test_path;
    test_path = gpu_test_dir_.Append(url);
    ASSERT_TRUE(file_util::PathExists(test_path))
        << "Missing test file: " << test_path.value();

    ui_test_utils::DOMMessageQueue message_queue;
    if (new_tab) {
      ui_test_utils::NavigateToURLWithDisposition(
          browser(), net::FilePathToFileURL(test_path),
          NEW_FOREGROUND_TAB, ui_test_utils::BROWSER_TEST_NONE);
    } else {
      ui_test_utils::NavigateToURL(
          browser(), net::FilePathToFileURL(test_path));
    }

    std::string result;
    ASSERT_TRUE(message_queue.WaitForMessage(&result));
    if (expected_reply)
      EXPECT_STREQ(expected_reply, result.c_str());
  }

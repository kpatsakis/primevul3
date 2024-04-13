void RunTestFunction(DevToolsWindow* window, const char* test_name) {
  std::string result;

  ASSERT_TRUE(
      content::ExecuteScriptAndExtractString(
          window->GetRenderViewHost(),
          "window.domAutomationController.send("
          "    '' + (window.uiTests && (typeof uiTests.runTest)));",
          &result));

  if (result == "function") {
    ASSERT_TRUE(
        content::ExecuteScriptAndExtractString(
            window->GetRenderViewHost(),
            base::StringPrintf("uiTests.runTest('%s')", test_name),
            &result));
    EXPECT_EQ("[OK]", result);
  } else {
    FAIL() << "DevTools front-end is broken.";
  }
}

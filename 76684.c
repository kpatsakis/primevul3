  void ExpectFieldValue(const std::wstring& field_name,
                        const std::string& expected_value) {
    std::string value;
    ASSERT_TRUE(ui_test_utils::ExecuteJavaScriptAndExtractString(
        browser()->GetSelectedWebContents()->GetRenderViewHost(), L"",
        L"window.domAutomationController.send("
        L"document.getElementById('" + field_name + L"').value);", &value));
    EXPECT_EQ(expected_value, value);
  }

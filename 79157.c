void DevToolsWindow::FileSavedAs(const std::string& url) {
  StringValue url_value(url);
  CallClientFunction("InspectorFrontendAPI.savedURL", &url_value, NULL, NULL);
}

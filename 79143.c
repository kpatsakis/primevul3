void DevToolsWindow::AppendedTo(const std::string& url) {
  StringValue url_value(url);
  CallClientFunction("InspectorFrontendAPI.appendedToURL", &url_value, NULL,
                     NULL);
}

void DevToolsWindow::CallClientFunction(const std::string& function_name,
                                        const Value* arg1,
                                        const Value* arg2,
                                        const Value* arg3) {
  std::string params;
  if (arg1) {
    std::string json;
    base::JSONWriter::Write(arg1, &json);
    params.append(json);
    if (arg2) {
      base::JSONWriter::Write(arg2, &json);
      params.append(", " + json);
      if (arg3) {
        base::JSONWriter::Write(arg3, &json);
        params.append(", " + json);
      }
    }
  }
  string16 javascript = ASCIIToUTF16(function_name + "(" + params + ");");
  web_contents_->GetRenderViewHost()->ExecuteJavascriptInWebFrame(
      base::string16(), javascript);
}

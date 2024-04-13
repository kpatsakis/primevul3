void RenderView::didExecuteCommand(const WebString& command_name) {
  const std::string& name = UTF16ToUTF8(command_name);
  if (StartsWithASCII(name, "Move", true) ||
      StartsWithASCII(name, "Insert", true) ||
      StartsWithASCII(name, "Delete", true))
    return;
  UserMetricsRecordAction(name);
}

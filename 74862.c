bool RenderView::RunJavaScriptMessage(int type,
                                      const std::wstring& message,
                                      const std::wstring& default_value,
                                      const GURL& frame_url,
                                      std::wstring* result) {
  bool success = false;
  std::wstring result_temp;
  if (!result)
    result = &result_temp;

  SendAndRunNestedMessageLoop(new ViewHostMsg_RunJavaScriptMessage(
      routing_id_, message, default_value, frame_url, type, &success, result));
  return success;
}

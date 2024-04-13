bool RenderView::runModalBeforeUnloadDialog(
    WebFrame* frame, const WebString& message) {
  bool success = false;
  std::wstring ignored_result;
  SendAndRunNestedMessageLoop(new ViewHostMsg_RunBeforeUnloadConfirm(
      routing_id_, frame->url(), UTF16ToWideHack(message), &success,
      &ignored_result));
  return success;
}

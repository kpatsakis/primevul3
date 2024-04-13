bool RenderView::runModalConfirmDialog(
    WebFrame* frame, const WebString& message) {
  return RunJavaScriptMessage(MessageBoxFlags::kIsJavascriptConfirm,
                              UTF16ToWideHack(message),
                              std::wstring(),
                              frame->url(),
                              NULL);
}

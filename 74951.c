void RenderView::runModalAlertDialog(
    WebFrame* frame, const WebString& message) {
  RunJavaScriptMessage(MessageBoxFlags::kIsJavascriptAlert,
                       UTF16ToWideHack(message),
                       std::wstring(),
                       frame->url(),
                       NULL);
}

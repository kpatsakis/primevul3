bool RenderView::runModalPromptDialog(
    WebFrame* frame, const WebString& message, const WebString& default_value,
    WebString* actual_value) {
  std::wstring result;
  bool ok = RunJavaScriptMessage(MessageBoxFlags::kIsJavascriptPrompt,
                                 UTF16ToWideHack(message),
                                 UTF16ToWideHack(default_value),
                                 frame->url(),
                                 &result);
  if (ok)
    actual_value->assign(WideToUTF16Hack(result));
  return ok;
}

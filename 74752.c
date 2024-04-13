void RenderView::EvaluateScript(const std::wstring& frame_xpath,
                                const std::wstring& script) {
  WebFrame* web_frame = GetChildFrame(frame_xpath);
  if (!web_frame)
    return;

  web_frame->executeScript(WebScriptSource(WideToUTF16Hack(script)));
}

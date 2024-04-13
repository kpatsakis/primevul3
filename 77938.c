bool RenderFrameImpl::runModalPromptDialog(
    const blink::WebString& message,
    const blink::WebString& default_value,
    blink::WebString* actual_value) {
  base::string16 result;
  bool ok = RunJavaScriptMessage(JAVASCRIPT_MESSAGE_TYPE_PROMPT,
                                 message,
                                 default_value,
                                 frame_->document().url(),
                                 &result);
  if (ok)
    actual_value->assign(result);
  return ok;
}

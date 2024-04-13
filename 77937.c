bool RenderFrameImpl::runModalConfirmDialog(const blink::WebString& message) {
  return RunJavaScriptMessage(JAVASCRIPT_MESSAGE_TYPE_CONFIRM,
                              message,
                              base::string16(),
                              frame_->document().url(),
                              NULL);
}

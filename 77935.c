void RenderFrameImpl::runModalAlertDialog(const blink::WebString& message) {
  RunJavaScriptMessage(JAVASCRIPT_MESSAGE_TYPE_ALERT,
                       message,
                       base::string16(),
                       frame_->document().url(),
                       NULL);
}

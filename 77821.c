void RenderFrameImpl::ExecuteJavaScript(const base::string16& javascript) {
  OnJavaScriptExecuteRequest(javascript, 0, false);
}

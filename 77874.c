bool RenderFrameImpl::RunJavaScriptMessage(JavaScriptMessageType type,
                                           const base::string16& message,
                                           const base::string16& default_value,
                                           const GURL& frame_url,
                                           base::string16* result) {
  if (render_view()->suppress_dialogs_until_swap_out_)
    return false;

  bool success = false;
  base::string16 result_temp;
  if (!result)
    result = &result_temp;

  render_view()->SendAndRunNestedMessageLoop(
      new FrameHostMsg_RunJavaScriptMessage(
        routing_id_, message, default_value, frame_url, type, &success,
        result));
  return success;
}

bool RenderFrameImpl::runModalBeforeUnloadDialog(
    bool is_reload,
    const blink::WebString& message) {
  if (render_view()->is_swapped_out_)
    return true;

  if (render_view()->suppress_dialogs_until_swap_out_)
    return false;

  bool success = false;
  base::string16 ignored_result;
  render_view()->SendAndRunNestedMessageLoop(
      new FrameHostMsg_RunBeforeUnloadConfirm(
          routing_id_, frame_->document().url(), message, is_reload,
          &success, &ignored_result));
  return success;
}

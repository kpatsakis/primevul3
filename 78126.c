void WebContentsImpl::OnDialogClosed(RenderViewHost* rvh,
                                     IPC::Message* reply_msg,
                                     bool success,
                                     const string16& user_input) {
  if (is_showing_before_unload_dialog_ && !success) {
    DidStopLoading(rvh);
    controller_.DiscardNonCommittedEntries();

    FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                      BeforeUnloadDialogCancelled());
  }
  is_showing_before_unload_dialog_ = false;
  static_cast<RenderViewHostImpl*>(
      rvh)->JavaScriptDialogClosed(reply_msg, success, user_input);
}

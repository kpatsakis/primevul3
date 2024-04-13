void WebContentsImpl::RunBeforeUnloadConfirm(RenderViewHost* rvh,
                                             const string16& message,
                                             bool is_reload,
                                             IPC::Message* reply_msg) {
  RenderViewHostImpl* rvhi = static_cast<RenderViewHostImpl*>(rvh);
  if (delegate_)
    delegate_->WillRunBeforeUnloadConfirm();

  bool suppress_this_message =
      rvhi->is_swapped_out() ||
      !delegate_ ||
      delegate_->ShouldSuppressDialogs() ||
      !delegate_->GetJavaScriptDialogManager();
  if (suppress_this_message) {
    rvhi->JavaScriptDialogClosed(reply_msg, true, string16());
    return;
  }

  is_showing_before_unload_dialog_ = true;
  dialog_manager_ = delegate_->GetJavaScriptDialogManager();
  dialog_manager_->RunBeforeUnloadDialog(
      this, message, is_reload,
      base::Bind(&WebContentsImpl::OnDialogClosed, base::Unretained(this), rvh,
                 reply_msg));
}

void WebContentsImpl::RunJavaScriptMessage(
    RenderViewHost* rvh,
    const string16& message,
    const string16& default_prompt,
    const GURL& frame_url,
    JavaScriptMessageType javascript_message_type,
    IPC::Message* reply_msg,
    bool* did_suppress_message) {
  bool suppress_this_message =
      static_cast<RenderViewHostImpl*>(rvh)->is_swapped_out() ||
      ShowingInterstitialPage() ||
      !delegate_ ||
      delegate_->ShouldSuppressDialogs() ||
      !delegate_->GetJavaScriptDialogManager();

  if (!suppress_this_message) {
    std::string accept_lang = GetContentClient()->browser()->
      GetAcceptLangs(GetBrowserContext());
    dialog_manager_ = delegate_->GetJavaScriptDialogManager();
    dialog_manager_->RunJavaScriptDialog(
        this,
        frame_url.GetOrigin(),
        accept_lang,
        javascript_message_type,
        message,
        default_prompt,
        base::Bind(&WebContentsImpl::OnDialogClosed,
                   base::Unretained(this),
                   rvh,
                   reply_msg),
        &suppress_this_message);
  }

  *did_suppress_message = suppress_this_message;

  if (suppress_this_message) {
    OnDialogClosed(rvh, reply_msg, false, string16());
  }

}

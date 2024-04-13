void InterstitialPage::TakeActionOnResourceDispatcher(
    ResourceRequestAction action) {
  DCHECK(ChromeThread::CurrentlyOn(ChromeThread::UI)) <<
      "TakeActionOnResourceDispatcher should be called on the main thread.";

  if (action == CANCEL || action == RESUME) {
    if (resource_dispatcher_host_notified_)
      return;
    resource_dispatcher_host_notified_ = true;
  }

  RenderViewHost* rvh = RenderViewHost::FromID(original_child_id_,
                                               original_rvh_id_);
  if (!rvh || !g_browser_process->resource_dispatcher_host())
    return;

  ChromeThread::PostTask(
      ChromeThread::IO, FROM_HERE,
      new ResourceRequestTask(original_child_id_, original_rvh_id_, action));
}

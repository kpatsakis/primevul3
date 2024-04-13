void WebContentsImpl::RenderViewTerminated(RenderViewHost* rvh,
                                           base::TerminationStatus status,
                                           int error_code) {
  if (rvh != GetRenderViewHost()) {
    return;
  }

  ClearPowerSaveBlockers(rvh);
  SetIsLoading(false, NULL);
  NotifyDisconnected();
  SetIsCrashed(status, error_code);
  GetView()->OnTabCrashed(GetCrashedStatus(), crashed_error_code_);

  FOR_EACH_OBSERVER(WebContentsObserver,
                    observers_,
                    RenderProcessGone(GetCrashedStatus()));
}

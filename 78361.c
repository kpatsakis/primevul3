void RenderThreadImpl::OnSetWebKitSharedTimersSuspended(bool suspend) {
  if (suspend_webkit_shared_timer_) {
    EnsureWebKitInitialized();
    if (suspend) {
      webkit_platform_support_->SuspendSharedTimer();
    } else {
      webkit_platform_support_->ResumeSharedTimer();
    }
    webkit_shared_timer_suspended_ = suspend;
  }
}

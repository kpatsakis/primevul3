void NavigationController::ContinuePendingReload() {
  if (pending_reload_ == NO_RELOAD) {
    NOTREACHED();
  } else {
    ReloadInternal(false, pending_reload_);
    pending_reload_ = NO_RELOAD;
  }
}

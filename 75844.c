void NavigationController::CancelPendingReload() {
  DCHECK(pending_reload_ != NO_RELOAD);
  pending_reload_ = NO_RELOAD;
}

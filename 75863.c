void NavigationController::ReloadIgnoringCache(bool check_for_repost) {
  ReloadInternal(check_for_repost, RELOAD_IGNORING_CACHE);
}

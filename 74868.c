void RenderView::StartNavStateSyncTimerIfNecessary() {
  int delay;
  if (send_content_state_immediately_)
    delay = 0;
  else if (is_hidden())
    delay = kDelaySecondsForContentStateSyncHidden;
  else
    delay = kDelaySecondsForContentStateSync;

  if (nav_state_sync_timer_.IsRunning()) {
    if (nav_state_sync_timer_.GetCurrentDelay().InSeconds() == delay)
      return;
    nav_state_sync_timer_.Stop();
  }

  nav_state_sync_timer_.Start(
      TimeDelta::FromSeconds(delay), this, &RenderView::SyncNavigationState);
}

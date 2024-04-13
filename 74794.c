void RenderView::OnEnablePreferredSizeChangedMode(int flags) {
  DCHECK(flags != kPreferredSizeNothing);
  if (send_preferred_size_changes_)
    return;
  send_preferred_size_changes_ = true;

  if (flags & kPreferredSizeHeightThisIsSlow) {
    preferred_size_change_timer_.Start(TimeDelta::FromMilliseconds(10), this,
                                       &RenderView::CheckPreferredSize);
  }
}

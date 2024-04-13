void BookmarkManagerView::WindowClosing() {
  g_browser_process->local_state()->SetInteger(
       prefs::kBookmarkManagerSplitLocation, split_view_->divider_x());
 }

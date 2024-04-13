void BookmarkManagerView::PrepareForShow() {
  int split_x = g_browser_process->local_state()->GetInteger(
      prefs::kBookmarkManagerSplitLocation);
  if (split_x == -1) {
    split_x = split_view_->width() / 3;
  }
  int min_split_size = split_view_->width() / 8;
  split_x = std::min(split_view_->width() - min_split_size,
                     std::max(min_split_size, split_x));
  split_view_->set_divider_x(split_x);
  if (!GetBookmarkModel()->IsLoaded()) {
    search_tf_->SetReadOnly(true);
    return;
  }

  LoadedImpl();
}

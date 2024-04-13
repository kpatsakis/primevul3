bool BookmarkManagerView::HandleKeystroke(
    views::TextField* sender,
    const views::TextField::Keystroke& key) {
  if (views::TextField::IsKeystrokeEnter(key)) {
    PerformSearch();
    search_tf_->SelectAll();
  }

  return false;
}

BookmarkManagerView::~BookmarkManagerView() {
  if (select_file_dialog_.get())
    select_file_dialog_->ListenerDestroyed();

  if (!GetBookmarkModel()->IsLoaded()) {
    GetBookmarkModel()->RemoveObserver(this);
  } else {
    table_view_->SetModel(NULL);
    tree_view_->SetModel(NULL);
  }
  manager = NULL;
  open_window = NULL;
}

void BookmarkManagerView::Loaded(BookmarkModel* model) {
  model->RemoveObserver(this);
  LoadedImpl();
}

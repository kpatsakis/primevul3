  explicit ImportObserverImpl(Profile* profile) : profile_(profile) {
    BookmarkModel* model = profile->GetBookmarkModel();
    initial_other_count_ = model->other_node()->GetChildCount();
  }

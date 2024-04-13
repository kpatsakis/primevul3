  virtual void ImportComplete() {
    MessageLoop::current()->DeleteSoon(FROM_HERE, this);

    BookmarkManagerView* manager = BookmarkManagerView::current();
    if (!manager || manager->profile() != profile_)
      return;

    BookmarkModel* model = profile_->GetBookmarkModel();
    int other_count = model->other_node()->GetChildCount();
    if (other_count == initial_other_count_ + 1) {
      BookmarkNode* imported_node =
          model->other_node()->GetChild(initial_other_count_);
      manager->SelectInTree(imported_node);
      manager->ExpandAll(imported_node);
    }
  }

void BookmarkManagerView::LoadedImpl() {
  BookmarkModel* bookmark_model = GetBookmarkModel();
  BookmarkNode* bookmark_bar_node = bookmark_model->GetBookmarkBarNode();
  table_model_.reset(
      BookmarkTableModel::CreateBookmarkTableModelForFolder(bookmark_model,
                                                            bookmark_bar_node));
  table_view_->SetModel(table_model_.get());
  table_view_->set_parent_node(bookmark_bar_node);

  tree_model_.reset(new BookmarkFolderTreeModel(bookmark_model));
  tree_view_->SetModel(tree_model_.get());

  tree_view_->ExpandAll();

  tree_view_->SetSelectedNode(
      tree_model_->GetFolderNodeForBookmarkNode(bookmark_bar_node));

  search_tf_->SetReadOnly(false);
  search_tf_->SetController(this);

  Layout();
  SchedulePaint();
}

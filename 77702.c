void BookmarkManagerView::OnTreeViewSelectionChanged(
    views::TreeView* tree_view) {
  TreeModelNode* node = tree_view_->GetSelectedNode();

  BookmarkTableModel* new_table_model = NULL;
  BookmarkNode* table_parent_node = NULL;
  bool is_search = false;

  if (node) {
    switch (tree_model_->GetNodeType(node)) {
      case BookmarkFolderTreeModel::BOOKMARK:
        table_parent_node = tree_model_->TreeNodeAsBookmarkNode(node);
        new_table_model =
            BookmarkTableModel::CreateBookmarkTableModelForFolder(
                profile_->GetBookmarkModel(),
                table_parent_node);
        break;

      case BookmarkFolderTreeModel::RECENTLY_BOOKMARKED:
        new_table_model = BookmarkTableModel::CreateRecentlyBookmarkedModel(
            profile_->GetBookmarkModel());
        break;

      case BookmarkFolderTreeModel::SEARCH:
        is_search = true;
        search_factory_.RevokeAll();
        new_table_model = CreateSearchTableModel();
        break;

      default:
        NOTREACHED();
        break;
    }
  }

  SetTableModel(new_table_model, table_parent_node, is_search);
}

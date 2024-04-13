void BookmarkManagerView::SelectInTree(BookmarkNode* node) {
  if (!node)
    return;

  BookmarkNode* parent = node->is_url() ? node->GetParent() : node;
  FolderNode* folder_node = tree_model_->GetFolderNodeForBookmarkNode(parent);
  if (!folder_node) {
    NOTREACHED();
    return;
  }

  tree_view_->SetSelectedNode(folder_node);

  if (node->is_url()) {
    int index = table_model_->IndexOfNode(node);
    if (index != -1)
      table_view_->Select(index);
    table_view_->RequestFocus();
  }
}

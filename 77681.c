void BookmarkManagerView::ExpandAll(BookmarkNode* node) {
  BookmarkNode* parent = node->is_url() ? node->GetParent() : node;
  FolderNode* folder_node = tree_model_->GetFolderNodeForBookmarkNode(parent);
  if (!folder_node) {
    NOTREACHED();
    return;
  }
  tree_view_->ExpandAll(folder_node);
}

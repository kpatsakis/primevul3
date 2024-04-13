void BookmarkManagerView::PerformSearch() {
  search_factory_.RevokeAll();
  tree_view_->SetController(NULL);
  tree_view_->SetSelectedNode(tree_model_->search_node());
  tree_view_->SetController(this);
  SetTableModel(CreateSearchTableModel(), NULL, true);
}

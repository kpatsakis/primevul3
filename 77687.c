std::vector<BookmarkNode*> BookmarkManagerView::GetSelectedTableNodes() {
  std::vector<BookmarkNode*> nodes;
  for (views::TableView::iterator i = table_view_->SelectionBegin();
       i != table_view_->SelectionEnd(); ++i) {
    nodes.push_back(table_model_->GetNodeForRow(*i));
  }
  std::reverse(nodes.begin(), nodes.end());
  return nodes;
}

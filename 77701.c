void BookmarkManagerView::OnTableViewDelete(views::TableView* table) {
  std::vector<BookmarkNode*> nodes = GetSelectedTableNodes();
  if (nodes.empty())
    return;
  for (size_t i = 0; i < nodes.size(); ++i) {
    GetBookmarkModel()->Remove(nodes[i]->GetParent(),
                               nodes[i]->GetParent()->IndexOfChild(nodes[i]));
  }
}

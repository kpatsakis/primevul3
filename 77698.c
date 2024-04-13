 void BookmarkManagerView::OnDoubleClick() {
   std::vector<BookmarkNode*> nodes = GetSelectedTableNodes();
   if (nodes.empty())
    return;
  if (nodes.size() == 1 && nodes[0]->is_folder()) {
    SelectInTree(nodes[0]);
    return;
  }
  bookmark_utils::OpenAll(
      GetWidget()->GetNativeView(), profile_, NULL, nodes, CURRENT_TAB);
}

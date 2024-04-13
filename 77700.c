void BookmarkManagerView::OnMiddleClick() {
  std::vector<BookmarkNode*> nodes = GetSelectedTableNodes();
  if (nodes.empty())
    return;
  if (nodes.size() == 1 && nodes[0]->is_folder()) {
    return;
  }

  bookmark_utils::OpenAll(
      GetWidget()->GetNativeView(), profile_, NULL, nodes, NEW_FOREGROUND_TAB);
}

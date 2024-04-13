void BookmarkManagerView::OnCutCopyPaste(CutCopyPasteType type,
                                         bool from_table) {
  if (type == CUT || type == COPY) {
    std::vector<BookmarkNode*> nodes;
    if (from_table) {
      nodes = GetSelectedTableNodes();
    } else {
      BookmarkNode* node = GetSelectedFolder();
      if (!node || node->GetParent() == GetBookmarkModel()->root_node())
        return;
      nodes.push_back(node);
    }
    if (nodes.empty())
      return;

    bookmark_utils::CopyToClipboard(GetBookmarkModel(), nodes, type == CUT);
  } else if (type == PASTE) {
    int index = from_table ? table_view_->FirstSelectedRow() : -1;
    if (index != -1)
      index++;
    bookmark_utils::PasteFromClipboard(GetBookmarkModel(), GetSelectedFolder(),
                                       index);
  }
}

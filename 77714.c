void BookmarkManagerView::ShowMenu(
    HWND host,
    int x,
    int y,
    BookmarkContextMenu::ConfigurationType config) {
  if (!GetBookmarkModel()->IsLoaded())
    return;

  if (config == BookmarkContextMenu::BOOKMARK_MANAGER_TABLE ||
      (config == BookmarkContextMenu::BOOKMARK_MANAGER_ORGANIZE_MENU &&
       table_view_->HasFocus())) {
    std::vector<BookmarkNode*> nodes = GetSelectedTableNodes();
    BookmarkNode* parent = GetSelectedFolder();
    if (!parent) {
      if (config == BookmarkContextMenu::BOOKMARK_MANAGER_TABLE)
        config = BookmarkContextMenu::BOOKMARK_MANAGER_TABLE_OTHER;
      else
        config = BookmarkContextMenu::BOOKMARK_MANAGER_ORGANIZE_MENU_OTHER;
    }
    BookmarkContextMenu menu(host, profile_, NULL, NULL, parent, nodes,
                             config);
    menu.RunMenuAt(x, y);
  } else {
    BookmarkNode* node = GetSelectedFolder();
    std::vector<BookmarkNode*> nodes;
    if (node)
      nodes.push_back(node);
    BookmarkContextMenu menu(GetWidget()->GetNativeView(), profile_, NULL, NULL,
                             node, nodes, config);
    menu.RunMenuAt(x, y);
  }
}

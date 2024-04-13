void BookmarkManagerView::ShowContextMenu(views::View* source,
                                          int x,
                                          int y,
                                          bool is_mouse_gesture) {
  DCHECK(source == table_view_ || source == tree_view_);
  bool is_table = (source == table_view_);
  ShowMenu(GetWidget()->GetNativeView(), x, y,
           is_table ? BookmarkContextMenu::BOOKMARK_MANAGER_TABLE :
                      BookmarkContextMenu::BOOKMARK_MANAGER_TREE);
}

void BookmarkManagerView::ShowToolsMenu(HWND host, int x, int y) {
  views::MenuItemView menu(this);
  menu.AppendMenuItemWithLabel(
          IDS_BOOKMARK_MANAGER_IMPORT_MENU,
          l10n_util::GetString(IDS_BOOKMARK_MANAGER_IMPORT_MENU));
  menu.AppendMenuItemWithLabel(
          IDS_BOOKMARK_MANAGER_EXPORT_MENU,
          l10n_util::GetString(IDS_BOOKMARK_MANAGER_EXPORT_MENU));
  views::MenuItemView::AnchorPosition anchor =
      UILayoutIsRightToLeft() ? views::MenuItemView::TOPRIGHT :
                                views::MenuItemView::TOPLEFT;
  menu.RunMenuAt(GetWidget()->GetNativeView(), gfx::Rect(x, y, 0, 0), anchor,
                 true);
}

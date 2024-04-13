gfx::Size BookmarkManagerView::GetPreferredSize() {
  return gfx::Size(views::Window::GetLocalizedContentsSize(
      IDS_BOOKMARK_MANAGER_DIALOG_WIDTH_CHARS,
      IDS_BOOKMARK_MANAGER_DIALOG_HEIGHT_LINES));
}

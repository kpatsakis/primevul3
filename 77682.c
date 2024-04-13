void BookmarkManagerView::FileSelected(const FilePath& path,
                                       int index,
                                       void* params) {
  int id = reinterpret_cast<int>(params);
  if (id == IDS_BOOKMARK_MANAGER_IMPORT_MENU) {
    ImporterHost* host = new ImporterHost();
    ProfileInfo profile_info;
    profile_info.browser_type = BOOKMARKS_HTML;
    profile_info.source_path = path.ToWStringHack();
    StartImportingWithUI(GetWidget()->GetNativeView(), FAVORITES, host,
                         profile_info, profile_,
                         new ImportObserverImpl(profile()), false);
  } else if (id == IDS_BOOKMARK_MANAGER_EXPORT_MENU) {
    if (g_browser_process->io_thread()) {
      bookmark_html_writer::WriteBookmarks(
          g_browser_process->io_thread()->message_loop(), GetBookmarkModel(),
          path.ToWStringHack());
    }
  } else {
    NOTREACHED();
  }
}

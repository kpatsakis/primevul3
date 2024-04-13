void BookmarkManagerView::ExecuteCommand(int id) {
  switch (id) {
    case IDS_BOOKMARK_MANAGER_IMPORT_MENU:
      UserMetrics::RecordAction(L"BookmarkManager_Import", profile_);
      ShowImportBookmarksFileChooser();
      break;

    case IDS_BOOKMARK_MANAGER_EXPORT_MENU:
      UserMetrics::RecordAction(L"BookmarkManager_Export", profile_);
      ShowExportBookmarksFileChooser();
      break;

    default:
      NOTREACHED();
      break;
  }
}

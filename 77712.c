void BookmarkManagerView::ShowExportBookmarksFileChooser() {
  if (select_file_dialog_.get())
    select_file_dialog_->ListenerDestroyed();

  SelectFileDialog::FileTypeInfo file_type_info;
  file_type_info.extensions.resize(1);
  file_type_info.extensions[0].push_back(FILE_PATH_LITERAL("html"));
  file_type_info.include_all_files = true;
  select_file_dialog_ = SelectFileDialog::Create(this);
  select_file_dialog_->SelectFile(
      SelectFileDialog::SELECT_SAVEAS_FILE, std::wstring(),
      FilePath(FILE_PATH_LITERAL("bookmarks.html")), &file_type_info, 0,
      L"html", GetWidget()->GetNativeView(),
      reinterpret_cast<void*>(IDS_BOOKMARK_MANAGER_EXPORT_MENU));
}

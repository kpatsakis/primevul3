void SavePackage::CreateDirectoryOnFileThread(
    const FilePath& website_save_dir,
    const FilePath& download_save_dir,
    bool skip_dir_check,
    const std::string& mime_type,
    const std::string& accept_langs) {
  FilePath save_dir;
  if (!skip_dir_check && !file_util::DirectoryExists(website_save_dir)) {
    if (!file_util::DirectoryExists(download_save_dir)) {
      bool res = file_util::CreateDirectory(download_save_dir);
      DCHECK(res);
    }
    save_dir = download_save_dir;
  } else {
    save_dir = website_save_dir;
  }

  bool can_save_as_complete = CanSaveAsComplete(mime_type);
  FilePath suggested_filename = GetSuggestedNameForSaveAs(
      can_save_as_complete, mime_type, accept_langs);
  FilePath::StringType pure_file_name =
      suggested_filename.RemoveExtension().BaseName().value();
  FilePath::StringType file_name_ext = suggested_filename.Extension();

  uint32 max_path = GetMaxPathLengthForDirectory(save_dir);

  if (GetSafePureFileName(save_dir, file_name_ext, max_path, &pure_file_name)) {
    save_dir = save_dir.Append(pure_file_name + file_name_ext);
  } else {
    save_dir = save_dir.Append(suggested_filename);
  }

  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(&SavePackage::ContinueGetSaveInfo, this, save_dir,
                 can_save_as_complete));
}

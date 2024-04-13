void SavePackage::ContinueGetSaveInfo(const FilePath& suggested_path,
                                      bool can_save_as_complete) {

  if (!web_contents() || !download_manager_->GetDelegate())
    return;

  FilePath::StringType default_extension;
  if (can_save_as_complete)
    default_extension = kDefaultHtmlExtension;

  download_manager_->GetDelegate()->ChooseSavePath(
      web_contents(),
      suggested_path,
      default_extension,
      can_save_as_complete,
      base::Bind(&SavePackage::OnPathPicked, AsWeakPtr()));
}

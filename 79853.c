void SavePackage::GetSaveInfo() {
  FilePath website_save_dir, download_save_dir;
  bool skip_dir_check;
  DCHECK(download_manager_);
  if (download_manager_->GetDelegate()) {
    download_manager_->GetDelegate()->GetSaveDir(
        web_contents()->GetBrowserContext(), &website_save_dir,
        &download_save_dir, &skip_dir_check);
  }
  std::string mime_type = web_contents()->GetContentsMimeType();
  std::string accept_languages =
      GetContentClient()->browser()->GetAcceptLangs(
          web_contents()->GetBrowserContext());

  BrowserThread::PostTask(
      BrowserThread::FILE, FROM_HERE,
      base::Bind(&SavePackage::CreateDirectoryOnFileThread, this,
          website_save_dir, download_save_dir, skip_dir_check,
          mime_type, accept_languages));
}

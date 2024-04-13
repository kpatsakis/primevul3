bool WebContentsImpl::SavePage(const base::FilePath& main_file,
                               const base::FilePath& dir_path,
                               SavePageType save_type) {
  Stop();

  save_package_ = new SavePackage(this, save_type, main_file, dir_path);
  return save_package_->Init(SavePackageDownloadCreatedCallback());
}

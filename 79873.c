void SavePackage::StopObservation() {
  DCHECK(download_);
  DCHECK(download_manager_);

  download_->RemoveObserver(this);
  download_ = NULL;
  download_manager_ = NULL;
}

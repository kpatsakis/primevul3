void ChromeDownloadManagerDelegate::GetReservedPath(
    DownloadItem& download,
    const FilePath& target_path,
    const FilePath& default_download_path,
    bool should_uniquify_path,
    const DownloadPathReservationTracker::ReservedPathCallback& callback) {
  DownloadPathReservationTracker::GetReservedPath(
      download, target_path, default_download_path, should_uniquify_path,
      callback);
}

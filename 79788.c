void ChromeDownloadManagerDelegate::ChooseDownloadPath(
    DownloadItem* item,
    const FilePath& suggested_path,
    const FileSelectedCallback& file_selected_callback) {
  DownloadFilePicker* file_picker =
#if defined(OS_CHROMEOS)
      new DownloadFilePickerChromeOS();
#else
      new DownloadFilePicker();
#endif
  file_picker->Init(download_manager_, item, suggested_path,
                    file_selected_callback);
}

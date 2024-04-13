void PPB_URLLoader_Impl::UpdateStatus() {
  if (status_callback_ &&
      (RecordDownloadProgress() || RecordUploadProgress())) {
    status_callback_(
        pp_instance(), pp_resource(),
        RecordUploadProgress() ? bytes_sent_ : -1,
        RecordUploadProgress() ?  total_bytes_to_be_sent_ : -1,
        RecordDownloadProgress() ? bytes_received_ : -1,
        RecordDownloadProgress() ? total_bytes_to_be_received_ : -1);
  }
}

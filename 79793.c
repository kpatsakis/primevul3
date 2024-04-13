void GenerateFileNameFromRequest(const DownloadItem& download_item,
                                 FilePath* generated_name,
                                 std::string referrer_charset) {
  std::string default_file_name(
      l10n_util::GetStringUTF8(IDS_DEFAULT_DOWNLOAD_FILENAME));

  *generated_name = net::GenerateFileName(download_item.GetURL(),
                                          download_item.GetContentDisposition(),
                                          referrer_charset,
                                          download_item.GetSuggestedFilename(),
                                          download_item.GetMimeType(),
                                          default_file_name);
}

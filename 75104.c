  void GetGeneratedFilename(const std::string& content_disposition,
                            const std::string& url,
                            const std::string& mime_type,
                            const std::string& referrer_charset,
                            std::wstring* generated_name_string) {
    DownloadCreateInfo info;
    info.content_disposition = content_disposition;
    info.url = GURL(url);
    info.mime_type = mime_type;
    info.referrer_charset = referrer_charset;
    FilePath generated_name;
    DownloadManager::GenerateFileNameFromInfo(&info, &generated_name);
    *generated_name_string = generated_name.ToWStringHack();
  }

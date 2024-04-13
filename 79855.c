FilePath SavePackage::GetSuggestedNameForSaveAs(
    bool can_save_as_complete,
    const std::string& contents_mime_type,
    const std::string& accept_langs) {
  FilePath name_with_proper_ext =
      FilePath::FromWStringHack(UTF16ToWideHack(title_));

  if (title_ == net::FormatUrl(page_url_, accept_langs)) {
    std::string url_path;
    if (!page_url_.SchemeIs(chrome::kDataScheme)) {
      std::vector<std::string> url_parts;
      base::SplitString(page_url_.path(), '/', &url_parts);
      if (!url_parts.empty()) {
        for (int i = static_cast<int>(url_parts.size()) - 1; i >= 0; --i) {
          url_path = url_parts[i];
          if (!url_path.empty())
            break;
        }
      }
      if (url_path.empty())
        url_path = page_url_.host();
    } else {
      url_path = "dataurl";
    }
    name_with_proper_ext = FilePath::FromWStringHack(UTF8ToWide(url_path));
  }

  name_with_proper_ext = EnsureMimeExtension(name_with_proper_ext,
                                             contents_mime_type);
  if (can_save_as_complete)
    name_with_proper_ext = EnsureHtmlExtension(name_with_proper_ext);

  FilePath::StringType file_name = name_with_proper_ext.value();
  file_util::ReplaceIllegalCharactersInPath(&file_name, ' ');
  return FilePath(file_name);
}

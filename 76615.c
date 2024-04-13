  bool CanRequestURL(const GURL& url) {
    SchemeMap::const_iterator judgment(scheme_policy_.find(url.scheme()));
    if (judgment != scheme_policy_.end())
      return judgment->second;

    if (url.SchemeIs(chrome::kFileScheme)) {
      FilePath path;
      if (net::FileURLToFilePath(url, &path))
        return request_file_set_.find(path) != request_file_set_.end();
    }

    return false;  // Unmentioned schemes are disallowed.
  }

  void GrantRequestOfSpecificFile(const FilePath &file) {
    request_file_set_.insert(file.StripTrailingSeparators());
  }

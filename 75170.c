  bool CanUploadFile(const FilePath& file) {
    return uploadable_files_.find(file) != uploadable_files_.end();
  }

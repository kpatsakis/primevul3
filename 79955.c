  bool HasFile(const FilePath& file) const {
    return files_.find(file.value()) != files_.end();
  }

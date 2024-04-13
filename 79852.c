bool SavePackage::GetSafePureFileName(const FilePath& dir_path,
                                      const FilePath::StringType& file_name_ext,
                                      uint32 max_file_path_len,
                                      FilePath::StringType* pure_file_name) {
  DCHECK(!pure_file_name->empty());
  int available_length = static_cast<int>(max_file_path_len -
                                          dir_path.value().length() -
                                          file_name_ext.length());
  if (!file_util::EndsWithSeparator(dir_path))
    --available_length;

  if (static_cast<int>(pure_file_name->length()) <= available_length)
    return true;

  if (available_length > 0) {
    *pure_file_name = pure_file_name->substr(0, available_length);
    return true;
  }

  pure_file_name->clear();
  return false;
}

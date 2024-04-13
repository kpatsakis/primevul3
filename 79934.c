bool NormalizeFilePath(const FilePath& path, FilePath* normalized_path) {
  FilePath real_path_result;
  if (!RealPath(path, &real_path_result))
    return false;

  stat_wrapper_t file_info;
  if (CallStat(real_path_result.value().c_str(), &file_info) != 0 ||
      S_ISDIR(file_info.st_mode))
    return false;

  *normalized_path = real_path_result;
  return true;
}

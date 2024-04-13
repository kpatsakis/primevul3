bool GetFileInfo(const FilePath& file_path, base::PlatformFileInfo* results) {
  stat_wrapper_t file_info;
  if (CallStat(file_path.value().c_str(), &file_info) != 0)
    return false;
  results->is_directory = S_ISDIR(file_info.st_mode);
  results->size = file_info.st_size;
  results->last_modified = base::Time::FromTimeT(file_info.st_mtime);
  results->last_accessed = base::Time::FromTimeT(file_info.st_atime);
  results->creation_time = base::Time::FromTimeT(file_info.st_ctime);
  return true;
}

bool GetPosixFilePermissions(const FilePath& path, int* mode) {
  base::ThreadRestrictions::AssertIOAllowed();
  DCHECK(mode);

  stat_wrapper_t file_info;
  if (CallStat(path.value().c_str(), &file_info) != 0)
    return false;

  *mode = file_info.st_mode & FILE_PERMISSION_MASK;
  return true;
}

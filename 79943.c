bool SetPosixFilePermissions(const FilePath& path,
                             int mode) {
  base::ThreadRestrictions::AssertIOAllowed();
  DCHECK((mode & ~FILE_PERMISSION_MASK) == 0);

  stat_wrapper_t stat_buf;
  if (CallStat(path.value().c_str(), &stat_buf) != 0)
    return false;

  mode_t updated_mode_bits = stat_buf.st_mode & ~FILE_PERMISSION_MASK;
  updated_mode_bits |= mode & FILE_PERMISSION_MASK;

  if (HANDLE_EINTR(chmod(path.value().c_str(), updated_mode_bits)) != 0)
    return false;

  return true;
}

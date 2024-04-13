bool GetShmemTempDir(FilePath* path, bool executable) {
#if defined(OS_LINUX)
  bool use_dev_shm = true;
  if (executable) {
    static const bool s_dev_shm_executable = DetermineDevShmExecutable();
    use_dev_shm = s_dev_shm_executable;
  }
  if (use_dev_shm) {
    *path = FilePath("/dev/shm");
    return true;
  }
#endif
  return GetTempDir(path);
}

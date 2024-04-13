bool DetermineDevShmExecutable() {
  bool result = false;
  FilePath path;
  int fd = CreateAndOpenFdForTemporaryFile(FilePath("/dev/shm"), &path);
  if (fd >= 0) {
    ScopedFD shm_fd_closer(&fd);
    Delete(path, false);
    long sysconf_result = sysconf(_SC_PAGESIZE);
    CHECK_GE(sysconf_result, 0);
    size_t pagesize = static_cast<size_t>(sysconf_result);
    CHECK_GE(sizeof(pagesize), sizeof(sysconf_result));
    void *mapping = mmap(NULL, pagesize, PROT_READ, MAP_SHARED, fd, 0);
    if (mapping != MAP_FAILED) {
      if (mprotect(mapping, pagesize, PROT_READ | PROT_EXEC) == 0)
        result = true;
      munmap(mapping, pagesize);
    }
  }
  return result;
}

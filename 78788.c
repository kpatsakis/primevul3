bool SharedMemory::PrepareMapFile(FILE *fp) {
  DCHECK_EQ(-1, mapped_file_);
  if (fp == NULL) return false;

  base::ThreadRestrictions::ScopedAllowIO allow_io;

  file_util::ScopedFILE file_closer(fp);

  mapped_file_ = dup(fileno(fp));
  if (mapped_file_ == -1) {
    if (errno == EMFILE) {
      LOG(WARNING) << "Shared memory creation failed; out of file descriptors";
      return false;
    } else {
      NOTREACHED() << "Call to dup failed, errno=" << errno;
    }
  }

  struct stat st;
  if (fstat(mapped_file_, &st))
    NOTREACHED();
  inode_ = st.st_ino;

  return true;
}

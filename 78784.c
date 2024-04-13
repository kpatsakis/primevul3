void SharedMemory::LockOrUnlockCommon(int function) {
  DCHECK_GE(mapped_file_, 0);
  while (lockf(mapped_file_, function, 0) < 0) {
    if (errno == EINTR) {
      continue;
    } else if (errno == ENOLCK) {
      base::PlatformThread::Sleep(base::TimeDelta::FromMilliseconds(500));
      continue;
    } else {
      NOTREACHED() << "lockf() failed."
                   << " function:" << function
                   << " fd:" << mapped_file_
                   << " errno:" << errno
                   << " msg:" << safe_strerror(errno);
    }
  }
}

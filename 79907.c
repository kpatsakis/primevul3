void MemoryMappedFile::CloseHandles() {
  base::ThreadRestrictions::AssertIOAllowed();

  if (data_ != NULL)
    munmap(data_, length_);
  if (file_ != base::kInvalidPlatformFileValue)
    ignore_result(HANDLE_EINTR(close(file_)));

  data_ = NULL;
  length_ = 0;
  file_ = base::kInvalidPlatformFileValue;
}

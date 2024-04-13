static size_t mz_zip_file_write_callback(void *pOpaque, mz_uint64 ofs,
                                         const void *pBuf, size_t n) {
  (void)ofs;
  return MZ_FWRITE(pBuf, 1, n, (MZ_FILE *)pOpaque);
}
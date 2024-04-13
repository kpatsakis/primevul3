mz_bool mz_zip_writer_add_mem(mz_zip_archive *pZip, const char *pArchive_name,
                              const void *pBuf, size_t buf_size,
                              mz_uint level_and_flags) {
  return mz_zip_writer_add_mem_ex(pZip, pArchive_name, pBuf, buf_size, NULL, 0,
                                  level_and_flags, 0, 0);
}
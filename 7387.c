mz_bool mz_zip_reader_extract_to_mem(mz_zip_archive *pZip, mz_uint file_index,
                                     void *pBuf, size_t buf_size,
                                     mz_uint flags) {
  return mz_zip_reader_extract_to_mem_no_alloc(pZip, file_index, pBuf, buf_size,
                                               flags, NULL, 0);
}
void *mz_zip_reader_extract_file_to_heap(mz_zip_archive *pZip,
                                         const char *pFilename, size_t *pSize,
                                         mz_uint flags) {
  int file_index = mz_zip_reader_locate_file(pZip, pFilename, NULL, flags);
  if (file_index < 0) {
    if (pSize) *pSize = 0;
    return MZ_FALSE;
  }
  return mz_zip_reader_extract_to_heap(pZip, file_index, pSize, flags);
}
mz_bool mz_zip_reader_extract_file_to_callback(mz_zip_archive *pZip,
                                               const char *pFilename,
                                               mz_file_write_func pCallback,
                                               void *pOpaque, mz_uint flags) {
  int file_index = mz_zip_reader_locate_file(pZip, pFilename, NULL, flags);
  if (file_index < 0) return MZ_FALSE;
  return mz_zip_reader_extract_to_callback(pZip, file_index, pCallback, pOpaque,
                                           flags);
}
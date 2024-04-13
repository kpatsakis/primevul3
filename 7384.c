mz_uint mz_zip_reader_get_filename(mz_zip_archive *pZip, mz_uint file_index,
                                   char *pFilename, mz_uint filename_buf_size) {
  mz_uint n;
  const mz_uint8 *p = mz_zip_reader_get_cdh(pZip, file_index);
  if (!p) {
    if (filename_buf_size) pFilename[0] = '\0';
    return 0;
  }
  n = MZ_READ_LE16(p + MZ_ZIP_CDH_FILENAME_LEN_OFS);
  if (filename_buf_size) {
    n = MZ_MIN(n, filename_buf_size - 1);
    memcpy(pFilename, p + MZ_ZIP_CENTRAL_DIR_HEADER_SIZE, n);
    pFilename[n] = '\0';
  }
  return n + 1;
}
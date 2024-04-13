mz_bool mz_zip_reader_extract_to_file(mz_zip_archive *pZip, mz_uint file_index,
                                      const char *pDst_filename,
                                      mz_uint flags) {
  mz_bool status;
  mz_zip_archive_file_stat file_stat;
  MZ_FILE *pFile;
  if (!mz_zip_reader_file_stat(pZip, file_index, &file_stat)) return MZ_FALSE;
  pFile = MZ_FOPEN(pDst_filename, "wb");
  if (!pFile) return MZ_FALSE;
  status = mz_zip_reader_extract_to_callback(
      pZip, file_index, mz_zip_file_write_callback, pFile, flags);
  if (MZ_FCLOSE(pFile) == EOF) return MZ_FALSE;
#ifndef MINIZ_NO_TIME
  if (status)
    mz_zip_set_file_times(pDst_filename, file_stat.m_time, file_stat.m_time);
#endif
  return status;
}
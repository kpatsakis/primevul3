mz_bool mz_zip_reader_extract_file_to_file(mz_zip_archive *pZip,
                                           const char *pArchive_filename,
                                           const char *pDst_filename,
                                           mz_uint flags) {
  int file_index =
      mz_zip_reader_locate_file(pZip, pArchive_filename, NULL, flags);
  if (file_index < 0) return MZ_FALSE;
  return mz_zip_reader_extract_to_file(pZip, file_index, pDst_filename, flags);
}
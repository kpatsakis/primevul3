void *mz_zip_extract_archive_file_to_heap(const char *pZip_filename,
                                          const char *pArchive_name,
                                          size_t *pSize, mz_uint flags) {
  int file_index;
  mz_zip_archive zip_archive;
  void *p = NULL;

  if (pSize) *pSize = 0;

  if ((!pZip_filename) || (!pArchive_name)) return NULL;

  MZ_CLEAR_OBJ(zip_archive);
  if (!mz_zip_reader_init_file(
          &zip_archive, pZip_filename,
          flags | MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY))
    return NULL;

  if ((file_index = mz_zip_reader_locate_file(&zip_archive, pArchive_name, NULL,
                                              flags)) >= 0)
    p = mz_zip_reader_extract_to_heap(&zip_archive, file_index, pSize, flags);

  mz_zip_reader_end(&zip_archive);
  return p;
}
mz_bool mz_zip_add_mem_to_archive_file_in_place(
    const char *pZip_filename, const char *pArchive_name, const void *pBuf,
    size_t buf_size, const void *pComment, mz_uint16 comment_size,
    mz_uint level_and_flags) {
  mz_bool status, created_new_archive = MZ_FALSE;
  mz_zip_archive zip_archive;
  struct MZ_FILE_STAT_STRUCT file_stat;
  MZ_CLEAR_OBJ(zip_archive);
  if ((int)level_and_flags < 0) level_and_flags = MZ_DEFAULT_LEVEL;
  if ((!pZip_filename) || (!pArchive_name) || ((buf_size) && (!pBuf)) ||
      ((comment_size) && (!pComment)) ||
      ((level_and_flags & 0xF) > MZ_UBER_COMPRESSION))
    return MZ_FALSE;
  if (!mz_zip_writer_validate_archive_name(pArchive_name)) return MZ_FALSE;
  if (MZ_FILE_STAT(pZip_filename, &file_stat) != 0) {
    // Create a new archive.
    if (!mz_zip_writer_init_file(&zip_archive, pZip_filename, 0))
      return MZ_FALSE;
    created_new_archive = MZ_TRUE;
  } else {
    // Append to an existing archive.
    if (!mz_zip_reader_init_file(
            &zip_archive, pZip_filename,
            level_and_flags | MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY))
      return MZ_FALSE;
    if (!mz_zip_writer_init_from_reader(&zip_archive, pZip_filename)) {
      mz_zip_reader_end(&zip_archive);
      return MZ_FALSE;
    }
  }
  status =
      mz_zip_writer_add_mem_ex(&zip_archive, pArchive_name, pBuf, buf_size,
                               pComment, comment_size, level_and_flags, 0, 0);
  // Always finalize, even if adding failed for some reason, so we have a valid
  // central directory. (This may not always succeed, but we can try.)
  if (!mz_zip_writer_finalize_archive(&zip_archive)) status = MZ_FALSE;
  if (!mz_zip_writer_end(&zip_archive)) status = MZ_FALSE;
  if ((!status) && (created_new_archive)) {
    // It's a new archive and something went wrong, so just delete it.
    int ignoredStatus = MZ_DELETE_FILE(pZip_filename);
    (void)ignoredStatus;
  }
  return status;
}
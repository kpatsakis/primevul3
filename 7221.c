mz_bool mz_zip_writer_init_from_reader(mz_zip_archive *pZip,
                                       const char *pFilename) {
  mz_zip_internal_state *pState;
  if ((!pZip) || (!pZip->m_pState) || (pZip->m_zip_mode != MZ_ZIP_MODE_READING))
    return MZ_FALSE;
  // No sense in trying to write to an archive that's already at the support max
  // size
  if ((pZip->m_total_files == 0xFFFF) ||
      ((pZip->m_archive_size + MZ_ZIP_CENTRAL_DIR_HEADER_SIZE +
        MZ_ZIP_LOCAL_DIR_HEADER_SIZE) > 0xFFFFFFFF))
    return MZ_FALSE;

  pState = pZip->m_pState;

  if (pState->m_pFile) {
#ifdef MINIZ_NO_STDIO
    pFilename;
    return MZ_FALSE;
#else
    // Archive is being read from stdio - try to reopen as writable.
    if (pZip->m_pIO_opaque != pZip) return MZ_FALSE;
    if (!pFilename) return MZ_FALSE;
    pZip->m_pWrite = mz_zip_file_write_func;
    if (NULL ==
        (pState->m_pFile = MZ_FREOPEN(pFilename, "r+b", pState->m_pFile))) {
      // The mz_zip_archive is now in a bogus state because pState->m_pFile is
      // NULL, so just close it.
      mz_zip_reader_end(pZip);
      return MZ_FALSE;
    }
#endif  // #ifdef MINIZ_NO_STDIO
  } else if (pState->m_pMem) {
    // Archive lives in a memory block. Assume it's from the heap that we can
    // resize using the realloc callback.
    if (pZip->m_pIO_opaque != pZip) return MZ_FALSE;
    pState->m_mem_capacity = pState->m_mem_size;
    pZip->m_pWrite = mz_zip_heap_write_func;
  }
  // Archive is being read via a user provided read function - make sure the
  // user has specified a write function too.
  else if (!pZip->m_pWrite)
    return MZ_FALSE;

  // Start writing new files at the archive's current central directory
  // location.
  pZip->m_archive_size = pZip->m_central_directory_file_ofs;
  pZip->m_zip_mode = MZ_ZIP_MODE_WRITING;
  pZip->m_central_directory_file_ofs = 0;

  return MZ_TRUE;
}
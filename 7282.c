mz_bool mz_zip_writer_init(mz_zip_archive *pZip, mz_uint64 existing_size) {
  if ((!pZip) || (pZip->m_pState) || (!pZip->m_pWrite) ||
      (pZip->m_zip_mode != MZ_ZIP_MODE_INVALID))
    return MZ_FALSE;

  if (pZip->m_file_offset_alignment) {
    // Ensure user specified file offset alignment is a power of 2.
    if (pZip->m_file_offset_alignment & (pZip->m_file_offset_alignment - 1))
      return MZ_FALSE;
  }

  if (!pZip->m_pAlloc) pZip->m_pAlloc = def_alloc_func;
  if (!pZip->m_pFree) pZip->m_pFree = def_free_func;
  if (!pZip->m_pRealloc) pZip->m_pRealloc = def_realloc_func;

  pZip->m_zip_mode = MZ_ZIP_MODE_WRITING;
  pZip->m_archive_size = existing_size;
  pZip->m_central_directory_file_ofs = 0;
  pZip->m_total_files = 0;

  if (NULL == (pZip->m_pState = (mz_zip_internal_state *)pZip->m_pAlloc(
                   pZip->m_pAlloc_opaque, 1, sizeof(mz_zip_internal_state))))
    return MZ_FALSE;
  memset(pZip->m_pState, 0, sizeof(mz_zip_internal_state));
  MZ_ZIP_ARRAY_SET_ELEMENT_SIZE(&pZip->m_pState->m_central_dir,
                                sizeof(mz_uint8));
  MZ_ZIP_ARRAY_SET_ELEMENT_SIZE(&pZip->m_pState->m_central_dir_offsets,
                                sizeof(mz_uint32));
  MZ_ZIP_ARRAY_SET_ELEMENT_SIZE(&pZip->m_pState->m_sorted_central_dir_offsets,
                                sizeof(mz_uint32));
  return MZ_TRUE;
}
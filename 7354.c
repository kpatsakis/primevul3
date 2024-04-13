static MZ_FORCEINLINE const mz_uint8 *mz_zip_reader_get_cdh(
    mz_zip_archive *pZip, mz_uint file_index) {
  if ((!pZip) || (!pZip->m_pState) || (file_index >= pZip->m_total_files) ||
      (pZip->m_zip_mode != MZ_ZIP_MODE_READING))
    return NULL;
  return &MZ_ZIP_ARRAY_ELEMENT(
      &pZip->m_pState->m_central_dir, mz_uint8,
      MZ_ZIP_ARRAY_ELEMENT(&pZip->m_pState->m_central_dir_offsets, mz_uint32,
                           file_index));
}
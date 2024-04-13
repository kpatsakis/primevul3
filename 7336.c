mz_bool mz_zip_reader_end(mz_zip_archive *pZip) {
  if ((!pZip) || (!pZip->m_pState) || (!pZip->m_pAlloc) || (!pZip->m_pFree) ||
      (pZip->m_zip_mode != MZ_ZIP_MODE_READING))
    return MZ_FALSE;

  if (pZip->m_pState) {
    mz_zip_internal_state *pState = pZip->m_pState;
    pZip->m_pState = NULL;
    mz_zip_array_clear(pZip, &pState->m_central_dir);
    mz_zip_array_clear(pZip, &pState->m_central_dir_offsets);
    mz_zip_array_clear(pZip, &pState->m_sorted_central_dir_offsets);

#ifndef MINIZ_NO_STDIO
    if (pState->m_pFile) {
      MZ_FCLOSE(pState->m_pFile);
      pState->m_pFile = NULL;
    }
#endif  // #ifndef MINIZ_NO_STDIO

    pZip->m_pFree(pZip->m_pAlloc_opaque, pState);
  }
  pZip->m_zip_mode = MZ_ZIP_MODE_INVALID;

  return MZ_TRUE;
}
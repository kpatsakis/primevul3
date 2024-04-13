int mz_zip_reader_locate_file(mz_zip_archive *pZip, const char *pName,
                              const char *pComment, mz_uint flags) {
  mz_uint file_index;
  size_t name_len, comment_len;
  if ((!pZip) || (!pZip->m_pState) || (!pName) ||
      (pZip->m_zip_mode != MZ_ZIP_MODE_READING))
    return -1;
  if (((flags & (MZ_ZIP_FLAG_IGNORE_PATH | MZ_ZIP_FLAG_CASE_SENSITIVE)) == 0) &&
      (!pComment) && (pZip->m_pState->m_sorted_central_dir_offsets.m_size))
    return mz_zip_reader_locate_file_binary_search(pZip, pName);
  name_len = strlen(pName);
  if (name_len > 0xFFFF) return -1;
  comment_len = pComment ? strlen(pComment) : 0;
  if (comment_len > 0xFFFF) return -1;
  for (file_index = 0; file_index < pZip->m_total_files; file_index++) {
    const mz_uint8 *pHeader = &MZ_ZIP_ARRAY_ELEMENT(
        &pZip->m_pState->m_central_dir, mz_uint8,
        MZ_ZIP_ARRAY_ELEMENT(&pZip->m_pState->m_central_dir_offsets, mz_uint32,
                             file_index));
    mz_uint filename_len = MZ_READ_LE16(pHeader + MZ_ZIP_CDH_FILENAME_LEN_OFS);
    const char *pFilename =
        (const char *)pHeader + MZ_ZIP_CENTRAL_DIR_HEADER_SIZE;
    if (filename_len < name_len) continue;
    if (comment_len) {
      mz_uint file_extra_len = MZ_READ_LE16(pHeader + MZ_ZIP_CDH_EXTRA_LEN_OFS),
              file_comment_len =
                  MZ_READ_LE16(pHeader + MZ_ZIP_CDH_COMMENT_LEN_OFS);
      const char *pFile_comment = pFilename + filename_len + file_extra_len;
      if ((file_comment_len != comment_len) ||
          (!mz_zip_reader_string_equal(pComment, pFile_comment,
                                       file_comment_len, flags)))
        continue;
    }
    if ((flags & MZ_ZIP_FLAG_IGNORE_PATH) && (filename_len)) {
      int ofs = filename_len - 1;
      do {
        if ((pFilename[ofs] == '/') || (pFilename[ofs] == '\\') ||
            (pFilename[ofs] == ':'))
          break;
      } while (--ofs >= 0);
      ofs++;
      pFilename += ofs;
      filename_len -= ofs;
    }
    if ((filename_len == name_len) &&
        (mz_zip_reader_string_equal(pName, pFilename, filename_len, flags)))
      return file_index;
  }
  return -1;
}
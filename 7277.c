static mz_bool mz_zip_writer_add_to_central_dir(
    mz_zip_archive *pZip, const char *pFilename, mz_uint16 filename_size,
    const void *pExtra, mz_uint16 extra_size, const void *pComment,
    mz_uint16 comment_size, mz_uint64 uncomp_size, mz_uint64 comp_size,
    mz_uint32 uncomp_crc32, mz_uint16 method, mz_uint16 bit_flags,
    mz_uint16 dos_time, mz_uint16 dos_date, mz_uint64 local_header_ofs,
    mz_uint32 ext_attributes) {
  mz_zip_internal_state *pState = pZip->m_pState;
  mz_uint32 central_dir_ofs = (mz_uint32)pState->m_central_dir.m_size;
  size_t orig_central_dir_size = pState->m_central_dir.m_size;
  mz_uint8 central_dir_header[MZ_ZIP_CENTRAL_DIR_HEADER_SIZE];

  // No zip64 support yet
  if ((local_header_ofs > 0xFFFFFFFF) ||
      (((mz_uint64)pState->m_central_dir.m_size +
        MZ_ZIP_CENTRAL_DIR_HEADER_SIZE + filename_size + extra_size +
        comment_size) > 0xFFFFFFFF))
    return MZ_FALSE;

  if (!mz_zip_writer_create_central_dir_header(
          pZip, central_dir_header, filename_size, extra_size, comment_size,
          uncomp_size, comp_size, uncomp_crc32, method, bit_flags, dos_time,
          dos_date, local_header_ofs, ext_attributes))
    return MZ_FALSE;

  if ((!mz_zip_array_push_back(pZip, &pState->m_central_dir, central_dir_header,
                               MZ_ZIP_CENTRAL_DIR_HEADER_SIZE)) ||
      (!mz_zip_array_push_back(pZip, &pState->m_central_dir, pFilename,
                               filename_size)) ||
      (!mz_zip_array_push_back(pZip, &pState->m_central_dir, pExtra,
                               extra_size)) ||
      (!mz_zip_array_push_back(pZip, &pState->m_central_dir, pComment,
                               comment_size)) ||
      (!mz_zip_array_push_back(pZip, &pState->m_central_dir_offsets,
                               &central_dir_ofs, 1))) {
    // Try to push the central directory array back into its original state.
    mz_zip_array_resize(pZip, &pState->m_central_dir, orig_central_dir_size,
                        MZ_FALSE);
    return MZ_FALSE;
  }

  return MZ_TRUE;
}
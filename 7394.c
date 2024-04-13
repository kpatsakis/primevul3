mz_bool mz_zip_writer_add_mem_ex(mz_zip_archive *pZip,
                                 const char *pArchive_name, const void *pBuf,
                                 size_t buf_size, const void *pComment,
                                 mz_uint16 comment_size,
                                 mz_uint level_and_flags, mz_uint64 uncomp_size,
                                 mz_uint32 uncomp_crc32) {
  mz_uint16 method = 0, dos_time = 0, dos_date = 0;
  mz_uint level, ext_attributes = 0, num_alignment_padding_bytes;
  mz_uint64 local_dir_header_ofs = pZip->m_archive_size,
            cur_archive_file_ofs = pZip->m_archive_size, comp_size = 0;
  size_t archive_name_size;
  mz_uint8 local_dir_header[MZ_ZIP_LOCAL_DIR_HEADER_SIZE];
  tdefl_compressor *pComp = NULL;
  mz_bool store_data_uncompressed;
  mz_zip_internal_state *pState;

  if ((int)level_and_flags < 0) level_and_flags = MZ_DEFAULT_LEVEL;
  level = level_and_flags & 0xF;
  store_data_uncompressed =
      ((!level) || (level_and_flags & MZ_ZIP_FLAG_COMPRESSED_DATA));

  if ((!pZip) || (!pZip->m_pState) ||
      (pZip->m_zip_mode != MZ_ZIP_MODE_WRITING) || ((buf_size) && (!pBuf)) ||
      (!pArchive_name) || ((comment_size) && (!pComment)) ||
      (pZip->m_total_files == 0xFFFF) || (level > MZ_UBER_COMPRESSION))
    return MZ_FALSE;

  pState = pZip->m_pState;

  if ((!(level_and_flags & MZ_ZIP_FLAG_COMPRESSED_DATA)) && (uncomp_size))
    return MZ_FALSE;
  // No zip64 support yet
  if ((buf_size > 0xFFFFFFFF) || (uncomp_size > 0xFFFFFFFF)) return MZ_FALSE;
  if (!mz_zip_writer_validate_archive_name(pArchive_name)) return MZ_FALSE;

#ifndef MINIZ_NO_TIME
  {
    time_t cur_time;
    time(&cur_time);
    mz_zip_time_to_dos_time(cur_time, &dos_time, &dos_date);
  }
#endif  // #ifndef MINIZ_NO_TIME

  archive_name_size = strlen(pArchive_name);
  if (archive_name_size > 0xFFFF) return MZ_FALSE;

  num_alignment_padding_bytes =
      mz_zip_writer_compute_padding_needed_for_file_alignment(pZip);

  // no zip64 support yet
  if ((pZip->m_total_files == 0xFFFF) ||
      ((pZip->m_archive_size + num_alignment_padding_bytes +
        MZ_ZIP_LOCAL_DIR_HEADER_SIZE + MZ_ZIP_CENTRAL_DIR_HEADER_SIZE +
        comment_size + archive_name_size) > 0xFFFFFFFF))
    return MZ_FALSE;

  if ((archive_name_size) && (pArchive_name[archive_name_size - 1] == '/')) {
    // Set DOS Subdirectory attribute bit.
    ext_attributes |= 0x10;
    // Subdirectories cannot contain data.
    if ((buf_size) || (uncomp_size)) return MZ_FALSE;
  }

  // Try to do any allocations before writing to the archive, so if an
  // allocation fails the file remains unmodified. (A good idea if we're doing
  // an in-place modification.)
  if ((!mz_zip_array_ensure_room(
          pZip, &pState->m_central_dir,
          MZ_ZIP_CENTRAL_DIR_HEADER_SIZE + archive_name_size + comment_size)) ||
      (!mz_zip_array_ensure_room(pZip, &pState->m_central_dir_offsets, 1)))
    return MZ_FALSE;

  if ((!store_data_uncompressed) && (buf_size)) {
    if (NULL == (pComp = (tdefl_compressor *)pZip->m_pAlloc(
                     pZip->m_pAlloc_opaque, 1, sizeof(tdefl_compressor))))
      return MZ_FALSE;
  }

  if (!mz_zip_writer_write_zeros(
          pZip, cur_archive_file_ofs,
          num_alignment_padding_bytes + sizeof(local_dir_header))) {
    pZip->m_pFree(pZip->m_pAlloc_opaque, pComp);
    return MZ_FALSE;
  }
  local_dir_header_ofs += num_alignment_padding_bytes;
  if (pZip->m_file_offset_alignment) {
    MZ_ASSERT((local_dir_header_ofs & (pZip->m_file_offset_alignment - 1)) ==
              0);
  }
  cur_archive_file_ofs +=
      num_alignment_padding_bytes + sizeof(local_dir_header);

  MZ_CLEAR_OBJ(local_dir_header);
  if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_file_ofs, pArchive_name,
                     archive_name_size) != archive_name_size) {
    pZip->m_pFree(pZip->m_pAlloc_opaque, pComp);
    return MZ_FALSE;
  }
  cur_archive_file_ofs += archive_name_size;

  if (!(level_and_flags & MZ_ZIP_FLAG_COMPRESSED_DATA)) {
    uncomp_crc32 =
        (mz_uint32)mz_crc32(MZ_CRC32_INIT, (const mz_uint8 *)pBuf, buf_size);
    uncomp_size = buf_size;
    if (uncomp_size <= 3) {
      level = 0;
      store_data_uncompressed = MZ_TRUE;
    }
  }

  if (store_data_uncompressed) {
    if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_file_ofs, pBuf,
                       buf_size) != buf_size) {
      pZip->m_pFree(pZip->m_pAlloc_opaque, pComp);
      return MZ_FALSE;
    }

    cur_archive_file_ofs += buf_size;
    comp_size = buf_size;

    if (level_and_flags & MZ_ZIP_FLAG_COMPRESSED_DATA) method = MZ_DEFLATED;
  } else if (buf_size) {
    mz_zip_writer_add_state state;

    state.m_pZip = pZip;
    state.m_cur_archive_file_ofs = cur_archive_file_ofs;
    state.m_comp_size = 0;

    if ((tdefl_init(pComp, mz_zip_writer_add_put_buf_callback, &state,
                    tdefl_create_comp_flags_from_zip_params(
                        level, -15, MZ_DEFAULT_STRATEGY)) !=
         TDEFL_STATUS_OKAY) ||
        (tdefl_compress_buffer(pComp, pBuf, buf_size, TDEFL_FINISH) !=
         TDEFL_STATUS_DONE)) {
      pZip->m_pFree(pZip->m_pAlloc_opaque, pComp);
      return MZ_FALSE;
    }

    comp_size = state.m_comp_size;
    cur_archive_file_ofs = state.m_cur_archive_file_ofs;

    method = MZ_DEFLATED;
  }

  pZip->m_pFree(pZip->m_pAlloc_opaque, pComp);
  pComp = NULL;

  // no zip64 support yet
  if ((comp_size > 0xFFFFFFFF) || (cur_archive_file_ofs > 0xFFFFFFFF))
    return MZ_FALSE;

  if (!mz_zip_writer_create_local_dir_header(
          pZip, local_dir_header, (mz_uint16)archive_name_size, 0, uncomp_size,
          comp_size, uncomp_crc32, method, 0, dos_time, dos_date))
    return MZ_FALSE;

  if (pZip->m_pWrite(pZip->m_pIO_opaque, local_dir_header_ofs, local_dir_header,
                     sizeof(local_dir_header)) != sizeof(local_dir_header))
    return MZ_FALSE;

  if (!mz_zip_writer_add_to_central_dir(
          pZip, pArchive_name, (mz_uint16)archive_name_size, NULL, 0, pComment,
          comment_size, uncomp_size, comp_size, uncomp_crc32, method, 0,
          dos_time, dos_date, local_dir_header_ofs, ext_attributes))
    return MZ_FALSE;

  pZip->m_total_files++;
  pZip->m_archive_size = cur_archive_file_ofs;

  return MZ_TRUE;
}
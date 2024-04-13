mz_bool mz_zip_writer_add_file(mz_zip_archive *pZip, const char *pArchive_name,
                               const char *pSrc_filename, const void *pComment,
                               mz_uint16 comment_size,
                               mz_uint level_and_flags) {
  mz_uint uncomp_crc32 = MZ_CRC32_INIT, level, num_alignment_padding_bytes;
  mz_uint16 method = 0, dos_time = 0, dos_date = 0, ext_attributes = 0;
  mz_uint64 local_dir_header_ofs = pZip->m_archive_size,
            cur_archive_file_ofs = pZip->m_archive_size, uncomp_size = 0,
            comp_size = 0;
  size_t archive_name_size;
  mz_uint8 local_dir_header[MZ_ZIP_LOCAL_DIR_HEADER_SIZE];
  MZ_FILE *pSrc_file = NULL;

  if ((int)level_and_flags < 0) level_and_flags = MZ_DEFAULT_LEVEL;
  level = level_and_flags & 0xF;

  if ((!pZip) || (!pZip->m_pState) ||
      (pZip->m_zip_mode != MZ_ZIP_MODE_WRITING) || (!pArchive_name) ||
      ((comment_size) && (!pComment)) || (level > MZ_UBER_COMPRESSION))
    return MZ_FALSE;
  if (level_and_flags & MZ_ZIP_FLAG_COMPRESSED_DATA) return MZ_FALSE;
  if (!mz_zip_writer_validate_archive_name(pArchive_name)) return MZ_FALSE;

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

  if (!mz_zip_get_file_modified_time(pSrc_filename, &dos_time, &dos_date))
    return MZ_FALSE;

  pSrc_file = MZ_FOPEN(pSrc_filename, "rb");
  if (!pSrc_file) return MZ_FALSE;
  MZ_FSEEK64(pSrc_file, 0, SEEK_END);
  uncomp_size = MZ_FTELL64(pSrc_file);
  MZ_FSEEK64(pSrc_file, 0, SEEK_SET);

  if (uncomp_size > 0xFFFFFFFF) {
    // No zip64 support yet
    MZ_FCLOSE(pSrc_file);
    return MZ_FALSE;
  }
  if (uncomp_size <= 3) level = 0;

  if (!mz_zip_writer_write_zeros(
          pZip, cur_archive_file_ofs,
          num_alignment_padding_bytes + sizeof(local_dir_header))) {
    MZ_FCLOSE(pSrc_file);
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
    MZ_FCLOSE(pSrc_file);
    return MZ_FALSE;
  }
  cur_archive_file_ofs += archive_name_size;

  if (uncomp_size) {
    mz_uint64 uncomp_remaining = uncomp_size;
    void *pRead_buf =
        pZip->m_pAlloc(pZip->m_pAlloc_opaque, 1, MZ_ZIP_MAX_IO_BUF_SIZE);
    if (!pRead_buf) {
      MZ_FCLOSE(pSrc_file);
      return MZ_FALSE;
    }

    if (!level) {
      while (uncomp_remaining) {
        mz_uint n =
            (mz_uint)MZ_MIN((mz_uint)MZ_ZIP_MAX_IO_BUF_SIZE, uncomp_remaining);
        if ((MZ_FREAD(pRead_buf, 1, n, pSrc_file) != n) ||
            (pZip->m_pWrite(pZip->m_pIO_opaque, cur_archive_file_ofs, pRead_buf,
                            n) != n)) {
          pZip->m_pFree(pZip->m_pAlloc_opaque, pRead_buf);
          MZ_FCLOSE(pSrc_file);
          return MZ_FALSE;
        }
        uncomp_crc32 =
            (mz_uint32)mz_crc32(uncomp_crc32, (const mz_uint8 *)pRead_buf, n);
        uncomp_remaining -= n;
        cur_archive_file_ofs += n;
      }
      comp_size = uncomp_size;
    } else {
      mz_bool result = MZ_FALSE;
      mz_zip_writer_add_state state;
      tdefl_compressor *pComp = (tdefl_compressor *)pZip->m_pAlloc(
          pZip->m_pAlloc_opaque, 1, sizeof(tdefl_compressor));
      if (!pComp) {
        pZip->m_pFree(pZip->m_pAlloc_opaque, pRead_buf);
        MZ_FCLOSE(pSrc_file);
        return MZ_FALSE;
      }

      state.m_pZip = pZip;
      state.m_cur_archive_file_ofs = cur_archive_file_ofs;
      state.m_comp_size = 0;

      if (tdefl_init(pComp, mz_zip_writer_add_put_buf_callback, &state,
                     tdefl_create_comp_flags_from_zip_params(
                         level, -15, MZ_DEFAULT_STRATEGY)) !=
          TDEFL_STATUS_OKAY) {
        pZip->m_pFree(pZip->m_pAlloc_opaque, pComp);
        pZip->m_pFree(pZip->m_pAlloc_opaque, pRead_buf);
        MZ_FCLOSE(pSrc_file);
        return MZ_FALSE;
      }

      for (;;) {
        size_t in_buf_size = (mz_uint32)MZ_MIN(uncomp_remaining,
                                               (mz_uint)MZ_ZIP_MAX_IO_BUF_SIZE);
        tdefl_status status;

        if (MZ_FREAD(pRead_buf, 1, in_buf_size, pSrc_file) != in_buf_size)
          break;

        uncomp_crc32 = (mz_uint32)mz_crc32(
            uncomp_crc32, (const mz_uint8 *)pRead_buf, in_buf_size);
        uncomp_remaining -= in_buf_size;

        status = tdefl_compress_buffer(
            pComp, pRead_buf, in_buf_size,
            uncomp_remaining ? TDEFL_NO_FLUSH : TDEFL_FINISH);
        if (status == TDEFL_STATUS_DONE) {
          result = MZ_TRUE;
          break;
        } else if (status != TDEFL_STATUS_OKAY)
          break;
      }

      pZip->m_pFree(pZip->m_pAlloc_opaque, pComp);

      if (!result) {
        pZip->m_pFree(pZip->m_pAlloc_opaque, pRead_buf);
        MZ_FCLOSE(pSrc_file);
        return MZ_FALSE;
      }

      comp_size = state.m_comp_size;
      cur_archive_file_ofs = state.m_cur_archive_file_ofs;

      method = MZ_DEFLATED;
    }

    pZip->m_pFree(pZip->m_pAlloc_opaque, pRead_buf);
  }

  MZ_FCLOSE(pSrc_file);
  pSrc_file = NULL;

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
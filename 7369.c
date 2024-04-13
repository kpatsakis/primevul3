mz_bool mz_zip_writer_add_from_zip_reader(mz_zip_archive *pZip,
                                          mz_zip_archive *pSource_zip,
                                          mz_uint file_index) {
  mz_uint n, bit_flags, num_alignment_padding_bytes;
  mz_uint64 comp_bytes_remaining, local_dir_header_ofs;
  mz_uint64 cur_src_file_ofs, cur_dst_file_ofs;
  mz_uint32
      local_header_u32[(MZ_ZIP_LOCAL_DIR_HEADER_SIZE + sizeof(mz_uint32) - 1) /
                       sizeof(mz_uint32)];
  mz_uint8 *pLocal_header = (mz_uint8 *)local_header_u32;
  mz_uint8 central_header[MZ_ZIP_CENTRAL_DIR_HEADER_SIZE];
  size_t orig_central_dir_size;
  mz_zip_internal_state *pState;
  void *pBuf;
  const mz_uint8 *pSrc_central_header;

  if ((!pZip) || (!pZip->m_pState) || (pZip->m_zip_mode != MZ_ZIP_MODE_WRITING))
    return MZ_FALSE;
  if (NULL ==
      (pSrc_central_header = mz_zip_reader_get_cdh(pSource_zip, file_index)))
    return MZ_FALSE;
  pState = pZip->m_pState;

  num_alignment_padding_bytes =
      mz_zip_writer_compute_padding_needed_for_file_alignment(pZip);

  // no zip64 support yet
  if ((pZip->m_total_files == 0xFFFF) ||
      ((pZip->m_archive_size + num_alignment_padding_bytes +
        MZ_ZIP_LOCAL_DIR_HEADER_SIZE + MZ_ZIP_CENTRAL_DIR_HEADER_SIZE) >
       0xFFFFFFFF))
    return MZ_FALSE;

  cur_src_file_ofs =
      MZ_READ_LE32(pSrc_central_header + MZ_ZIP_CDH_LOCAL_HEADER_OFS);
  cur_dst_file_ofs = pZip->m_archive_size;

  if (pSource_zip->m_pRead(pSource_zip->m_pIO_opaque, cur_src_file_ofs,
                           pLocal_header, MZ_ZIP_LOCAL_DIR_HEADER_SIZE) !=
      MZ_ZIP_LOCAL_DIR_HEADER_SIZE)
    return MZ_FALSE;
  if (MZ_READ_LE32(pLocal_header) != MZ_ZIP_LOCAL_DIR_HEADER_SIG)
    return MZ_FALSE;
  cur_src_file_ofs += MZ_ZIP_LOCAL_DIR_HEADER_SIZE;

  if (!mz_zip_writer_write_zeros(pZip, cur_dst_file_ofs,
                                 num_alignment_padding_bytes))
    return MZ_FALSE;
  cur_dst_file_ofs += num_alignment_padding_bytes;
  local_dir_header_ofs = cur_dst_file_ofs;
  if (pZip->m_file_offset_alignment) {
    MZ_ASSERT((local_dir_header_ofs & (pZip->m_file_offset_alignment - 1)) ==
              0);
  }

  if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_dst_file_ofs, pLocal_header,
                     MZ_ZIP_LOCAL_DIR_HEADER_SIZE) !=
      MZ_ZIP_LOCAL_DIR_HEADER_SIZE)
    return MZ_FALSE;
  cur_dst_file_ofs += MZ_ZIP_LOCAL_DIR_HEADER_SIZE;

  n = MZ_READ_LE16(pLocal_header + MZ_ZIP_LDH_FILENAME_LEN_OFS) +
      MZ_READ_LE16(pLocal_header + MZ_ZIP_LDH_EXTRA_LEN_OFS);
  comp_bytes_remaining =
      n + MZ_READ_LE32(pSrc_central_header + MZ_ZIP_CDH_COMPRESSED_SIZE_OFS);

  if (NULL == (pBuf = pZip->m_pAlloc(
                   pZip->m_pAlloc_opaque, 1,
                   (size_t)MZ_MAX(sizeof(mz_uint32) * 4,
                                  MZ_MIN((mz_uint)MZ_ZIP_MAX_IO_BUF_SIZE,
                                         comp_bytes_remaining)))))
    return MZ_FALSE;

  while (comp_bytes_remaining) {
    n = (mz_uint)MZ_MIN((mz_uint)MZ_ZIP_MAX_IO_BUF_SIZE, comp_bytes_remaining);
    if (pSource_zip->m_pRead(pSource_zip->m_pIO_opaque, cur_src_file_ofs, pBuf,
                             n) != n) {
      pZip->m_pFree(pZip->m_pAlloc_opaque, pBuf);
      return MZ_FALSE;
    }
    cur_src_file_ofs += n;

    if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_dst_file_ofs, pBuf, n) != n) {
      pZip->m_pFree(pZip->m_pAlloc_opaque, pBuf);
      return MZ_FALSE;
    }
    cur_dst_file_ofs += n;

    comp_bytes_remaining -= n;
  }

  bit_flags = MZ_READ_LE16(pLocal_header + MZ_ZIP_LDH_BIT_FLAG_OFS);
  if (bit_flags & 8) {
    // Copy data descriptor
    if (pSource_zip->m_pRead(pSource_zip->m_pIO_opaque, cur_src_file_ofs, pBuf,
                             sizeof(mz_uint32) * 4) != sizeof(mz_uint32) * 4) {
      pZip->m_pFree(pZip->m_pAlloc_opaque, pBuf);
      return MZ_FALSE;
    }

    n = sizeof(mz_uint32) * ((MZ_READ_LE32(pBuf) == 0x08074b50) ? 4 : 3);
    if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_dst_file_ofs, pBuf, n) != n) {
      pZip->m_pFree(pZip->m_pAlloc_opaque, pBuf);
      return MZ_FALSE;
    }

    cur_src_file_ofs += n;
    cur_dst_file_ofs += n;
  }
  pZip->m_pFree(pZip->m_pAlloc_opaque, pBuf);

  // no zip64 support yet
  if (cur_dst_file_ofs > 0xFFFFFFFF) return MZ_FALSE;

  orig_central_dir_size = pState->m_central_dir.m_size;

  memcpy(central_header, pSrc_central_header, MZ_ZIP_CENTRAL_DIR_HEADER_SIZE);
  MZ_WRITE_LE32(central_header + MZ_ZIP_CDH_LOCAL_HEADER_OFS,
                local_dir_header_ofs);
  if (!mz_zip_array_push_back(pZip, &pState->m_central_dir, central_header,
                              MZ_ZIP_CENTRAL_DIR_HEADER_SIZE))
    return MZ_FALSE;

  n = MZ_READ_LE16(pSrc_central_header + MZ_ZIP_CDH_FILENAME_LEN_OFS) +
      MZ_READ_LE16(pSrc_central_header + MZ_ZIP_CDH_EXTRA_LEN_OFS) +
      MZ_READ_LE16(pSrc_central_header + MZ_ZIP_CDH_COMMENT_LEN_OFS);
  if (!mz_zip_array_push_back(
          pZip, &pState->m_central_dir,
          pSrc_central_header + MZ_ZIP_CENTRAL_DIR_HEADER_SIZE, n)) {
    mz_zip_array_resize(pZip, &pState->m_central_dir, orig_central_dir_size,
                        MZ_FALSE);
    return MZ_FALSE;
  }

  if (pState->m_central_dir.m_size > 0xFFFFFFFF) return MZ_FALSE;
  n = (mz_uint32)orig_central_dir_size;
  if (!mz_zip_array_push_back(pZip, &pState->m_central_dir_offsets, &n, 1)) {
    mz_zip_array_resize(pZip, &pState->m_central_dir, orig_central_dir_size,
                        MZ_FALSE);
    return MZ_FALSE;
  }

  pZip->m_total_files++;
  pZip->m_archive_size = cur_dst_file_ofs;

  return MZ_TRUE;
}
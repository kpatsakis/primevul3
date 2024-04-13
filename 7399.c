mz_bool mz_zip_reader_extract_to_callback(mz_zip_archive *pZip,
                                          mz_uint file_index,
                                          mz_file_write_func pCallback,
                                          void *pOpaque, mz_uint flags) {
  int status = TINFL_STATUS_DONE;
  mz_uint file_crc32 = MZ_CRC32_INIT;
  mz_uint64 read_buf_size, read_buf_ofs = 0, read_buf_avail, comp_remaining,
                           out_buf_ofs = 0, cur_file_ofs;
  mz_zip_archive_file_stat file_stat;
  void *pRead_buf = NULL;
  void *pWrite_buf = NULL;
  mz_uint32
      local_header_u32[(MZ_ZIP_LOCAL_DIR_HEADER_SIZE + sizeof(mz_uint32) - 1) /
                       sizeof(mz_uint32)];
  mz_uint8 *pLocal_header = (mz_uint8 *)local_header_u32;

  if (!mz_zip_reader_file_stat(pZip, file_index, &file_stat)) return MZ_FALSE;

  // Empty file, or a directory (but not always a directory - I've seen odd zips
  // with directories that have compressed data which inflates to 0 bytes)
  if (!file_stat.m_comp_size) return MZ_TRUE;

  // Entry is a subdirectory (I've seen old zips with dir entries which have
  // compressed deflate data which inflates to 0 bytes, but these entries claim
  // to uncompress to 512 bytes in the headers).
  // I'm torn how to handle this case - should it fail instead?
  if (mz_zip_reader_is_file_a_directory(pZip, file_index)) return MZ_TRUE;

  // Encryption and patch files are not supported.
  if (file_stat.m_bit_flag & (1 | 32)) return MZ_FALSE;

  // This function only supports stored and deflate.
  if ((!(flags & MZ_ZIP_FLAG_COMPRESSED_DATA)) && (file_stat.m_method != 0) &&
      (file_stat.m_method != MZ_DEFLATED))
    return MZ_FALSE;

  // Read and parse the local directory entry.
  cur_file_ofs = file_stat.m_local_header_ofs;
  if (pZip->m_pRead(pZip->m_pIO_opaque, cur_file_ofs, pLocal_header,
                    MZ_ZIP_LOCAL_DIR_HEADER_SIZE) !=
      MZ_ZIP_LOCAL_DIR_HEADER_SIZE)
    return MZ_FALSE;
  if (MZ_READ_LE32(pLocal_header) != MZ_ZIP_LOCAL_DIR_HEADER_SIG)
    return MZ_FALSE;

  cur_file_ofs += MZ_ZIP_LOCAL_DIR_HEADER_SIZE +
                  MZ_READ_LE16(pLocal_header + MZ_ZIP_LDH_FILENAME_LEN_OFS) +
                  MZ_READ_LE16(pLocal_header + MZ_ZIP_LDH_EXTRA_LEN_OFS);
  if ((cur_file_ofs + file_stat.m_comp_size) > pZip->m_archive_size)
    return MZ_FALSE;

  // Decompress the file either directly from memory or from a file input
  // buffer.
  if (pZip->m_pState->m_pMem) {
    pRead_buf = (mz_uint8 *)pZip->m_pState->m_pMem + cur_file_ofs;
    read_buf_size = read_buf_avail = file_stat.m_comp_size;
    comp_remaining = 0;
  } else {
    read_buf_size =
        MZ_MIN(file_stat.m_comp_size, (mz_uint)MZ_ZIP_MAX_IO_BUF_SIZE);
    if (NULL == (pRead_buf = pZip->m_pAlloc(pZip->m_pAlloc_opaque, 1,
                                            (size_t)read_buf_size)))
      return MZ_FALSE;
    read_buf_avail = 0;
    comp_remaining = file_stat.m_comp_size;
  }

  if ((flags & MZ_ZIP_FLAG_COMPRESSED_DATA) || (!file_stat.m_method)) {
    // The file is stored or the caller has requested the compressed data.
    if (pZip->m_pState->m_pMem) {
#ifdef _MSC_VER
      if (((0, sizeof(size_t) == sizeof(mz_uint32))) &&
          (file_stat.m_comp_size > 0xFFFFFFFF))
#else
      if (((sizeof(size_t) == sizeof(mz_uint32))) &&
          (file_stat.m_comp_size > 0xFFFFFFFF))
#endif
        return MZ_FALSE;
      if (pCallback(pOpaque, out_buf_ofs, pRead_buf,
                    (size_t)file_stat.m_comp_size) != file_stat.m_comp_size)
        status = TINFL_STATUS_FAILED;
      else if (!(flags & MZ_ZIP_FLAG_COMPRESSED_DATA))
        file_crc32 =
            (mz_uint32)mz_crc32(file_crc32, (const mz_uint8 *)pRead_buf,
                                (size_t)file_stat.m_comp_size);
      cur_file_ofs += file_stat.m_comp_size;
      out_buf_ofs += file_stat.m_comp_size;
      comp_remaining = 0;
    } else {
      while (comp_remaining) {
        read_buf_avail = MZ_MIN(read_buf_size, comp_remaining);
        if (pZip->m_pRead(pZip->m_pIO_opaque, cur_file_ofs, pRead_buf,
                          (size_t)read_buf_avail) != read_buf_avail) {
          status = TINFL_STATUS_FAILED;
          break;
        }

        if (!(flags & MZ_ZIP_FLAG_COMPRESSED_DATA))
          file_crc32 = (mz_uint32)mz_crc32(
              file_crc32, (const mz_uint8 *)pRead_buf, (size_t)read_buf_avail);

        if (pCallback(pOpaque, out_buf_ofs, pRead_buf,
                      (size_t)read_buf_avail) != read_buf_avail) {
          status = TINFL_STATUS_FAILED;
          break;
        }
        cur_file_ofs += read_buf_avail;
        out_buf_ofs += read_buf_avail;
        comp_remaining -= read_buf_avail;
      }
    }
  } else {
    tinfl_decompressor inflator;
    tinfl_init(&inflator);

    if (NULL == (pWrite_buf = pZip->m_pAlloc(pZip->m_pAlloc_opaque, 1,
                                             TINFL_LZ_DICT_SIZE)))
      status = TINFL_STATUS_FAILED;
    else {
      do {
        mz_uint8 *pWrite_buf_cur =
            (mz_uint8 *)pWrite_buf + (out_buf_ofs & (TINFL_LZ_DICT_SIZE - 1));
        size_t in_buf_size,
            out_buf_size =
                TINFL_LZ_DICT_SIZE - (out_buf_ofs & (TINFL_LZ_DICT_SIZE - 1));
        if ((!read_buf_avail) && (!pZip->m_pState->m_pMem)) {
          read_buf_avail = MZ_MIN(read_buf_size, comp_remaining);
          if (pZip->m_pRead(pZip->m_pIO_opaque, cur_file_ofs, pRead_buf,
                            (size_t)read_buf_avail) != read_buf_avail) {
            status = TINFL_STATUS_FAILED;
            break;
          }
          cur_file_ofs += read_buf_avail;
          comp_remaining -= read_buf_avail;
          read_buf_ofs = 0;
        }

        in_buf_size = (size_t)read_buf_avail;
        status = tinfl_decompress(
            &inflator, (const mz_uint8 *)pRead_buf + read_buf_ofs, &in_buf_size,
            (mz_uint8 *)pWrite_buf, pWrite_buf_cur, &out_buf_size,
            comp_remaining ? TINFL_FLAG_HAS_MORE_INPUT : 0);
        read_buf_avail -= in_buf_size;
        read_buf_ofs += in_buf_size;

        if (out_buf_size) {
          if (pCallback(pOpaque, out_buf_ofs, pWrite_buf_cur, out_buf_size) !=
              out_buf_size) {
            status = TINFL_STATUS_FAILED;
            break;
          }
          file_crc32 =
              (mz_uint32)mz_crc32(file_crc32, pWrite_buf_cur, out_buf_size);
          if ((out_buf_ofs += out_buf_size) > file_stat.m_uncomp_size) {
            status = TINFL_STATUS_FAILED;
            break;
          }
        }
      } while ((status == TINFL_STATUS_NEEDS_MORE_INPUT) ||
               (status == TINFL_STATUS_HAS_MORE_OUTPUT));
    }
  }

  if ((status == TINFL_STATUS_DONE) &&
      (!(flags & MZ_ZIP_FLAG_COMPRESSED_DATA))) {
    // Make sure the entire file was decompressed, and check its CRC.
    if ((out_buf_ofs != file_stat.m_uncomp_size) ||
        (file_crc32 != file_stat.m_crc32))
      status = TINFL_STATUS_FAILED;
  }

  if (!pZip->m_pState->m_pMem) pZip->m_pFree(pZip->m_pAlloc_opaque, pRead_buf);
  if (pWrite_buf) pZip->m_pFree(pZip->m_pAlloc_opaque, pWrite_buf);

  return status == TINFL_STATUS_DONE;
}
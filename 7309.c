static int tdefl_flush_block(tdefl_compressor *d, int flush) {
  mz_uint saved_bit_buf, saved_bits_in;
  mz_uint8 *pSaved_output_buf;
  mz_bool comp_block_succeeded = MZ_FALSE;
  int n, use_raw_block =
             ((d->m_flags & TDEFL_FORCE_ALL_RAW_BLOCKS) != 0) &&
             (d->m_lookahead_pos - d->m_lz_code_buf_dict_pos) <= d->m_dict_size;
  mz_uint8 *pOutput_buf_start =
      ((d->m_pPut_buf_func == NULL) &&
       ((*d->m_pOut_buf_size - d->m_out_buf_ofs) >= TDEFL_OUT_BUF_SIZE))
          ? ((mz_uint8 *)d->m_pOut_buf + d->m_out_buf_ofs)
          : d->m_output_buf;

  d->m_pOutput_buf = pOutput_buf_start;
  d->m_pOutput_buf_end = d->m_pOutput_buf + TDEFL_OUT_BUF_SIZE - 16;

  MZ_ASSERT(!d->m_output_flush_remaining);
  d->m_output_flush_ofs = 0;
  d->m_output_flush_remaining = 0;

  *d->m_pLZ_flags = (mz_uint8)(*d->m_pLZ_flags >> d->m_num_flags_left);
  d->m_pLZ_code_buf -= (d->m_num_flags_left == 8);

  if ((d->m_flags & TDEFL_WRITE_ZLIB_HEADER) && (!d->m_block_index)) {
    TDEFL_PUT_BITS(0x78, 8);
    TDEFL_PUT_BITS(0x01, 8);
  }

  TDEFL_PUT_BITS(flush == TDEFL_FINISH, 1);

  pSaved_output_buf = d->m_pOutput_buf;
  saved_bit_buf = d->m_bit_buffer;
  saved_bits_in = d->m_bits_in;

  if (!use_raw_block)
    comp_block_succeeded =
        tdefl_compress_block(d, (d->m_flags & TDEFL_FORCE_ALL_STATIC_BLOCKS) ||
                                    (d->m_total_lz_bytes < 48));

  // If the block gets expanded, forget the current contents of the output
  // buffer and send a raw block instead.
  if (((use_raw_block) ||
       ((d->m_total_lz_bytes) && ((d->m_pOutput_buf - pSaved_output_buf + 1U) >=
                                  d->m_total_lz_bytes))) &&
      ((d->m_lookahead_pos - d->m_lz_code_buf_dict_pos) <= d->m_dict_size)) {
    mz_uint i;
    d->m_pOutput_buf = pSaved_output_buf;
    d->m_bit_buffer = saved_bit_buf, d->m_bits_in = saved_bits_in;
    TDEFL_PUT_BITS(0, 2);
    if (d->m_bits_in) {
      TDEFL_PUT_BITS(0, 8 - d->m_bits_in);
    }
    for (i = 2; i; --i, d->m_total_lz_bytes ^= 0xFFFF) {
      TDEFL_PUT_BITS(d->m_total_lz_bytes & 0xFFFF, 16);
    }
    for (i = 0; i < d->m_total_lz_bytes; ++i) {
      TDEFL_PUT_BITS(
          d->m_dict[(d->m_lz_code_buf_dict_pos + i) & TDEFL_LZ_DICT_SIZE_MASK],
          8);
    }
  }
  // Check for the extremely unlikely (if not impossible) case of the compressed
  // block not fitting into the output buffer when using dynamic codes.
  else if (!comp_block_succeeded) {
    d->m_pOutput_buf = pSaved_output_buf;
    d->m_bit_buffer = saved_bit_buf, d->m_bits_in = saved_bits_in;
    tdefl_compress_block(d, MZ_TRUE);
  }

  if (flush) {
    if (flush == TDEFL_FINISH) {
      if (d->m_bits_in) {
        TDEFL_PUT_BITS(0, 8 - d->m_bits_in);
      }
      if (d->m_flags & TDEFL_WRITE_ZLIB_HEADER) {
        mz_uint i, a = d->m_adler32;
        for (i = 0; i < 4; i++) {
          TDEFL_PUT_BITS((a >> 24) & 0xFF, 8);
          a <<= 8;
        }
      }
    } else {
      mz_uint i, z = 0;
      TDEFL_PUT_BITS(0, 3);
      if (d->m_bits_in) {
        TDEFL_PUT_BITS(0, 8 - d->m_bits_in);
      }
      for (i = 2; i; --i, z ^= 0xFFFF) {
        TDEFL_PUT_BITS(z & 0xFFFF, 16);
      }
    }
  }

  MZ_ASSERT(d->m_pOutput_buf < d->m_pOutput_buf_end);

  memset(&d->m_huff_count[0][0], 0,
         sizeof(d->m_huff_count[0][0]) * TDEFL_MAX_HUFF_SYMBOLS_0);
  memset(&d->m_huff_count[1][0], 0,
         sizeof(d->m_huff_count[1][0]) * TDEFL_MAX_HUFF_SYMBOLS_1);

  d->m_pLZ_code_buf = d->m_lz_code_buf + 1;
  d->m_pLZ_flags = d->m_lz_code_buf;
  d->m_num_flags_left = 8;
  d->m_lz_code_buf_dict_pos += d->m_total_lz_bytes;
  d->m_total_lz_bytes = 0;
  d->m_block_index++;

  if ((n = (int)(d->m_pOutput_buf - pOutput_buf_start)) != 0) {
    if (d->m_pPut_buf_func) {
      *d->m_pIn_buf_size = d->m_pSrc - (const mz_uint8 *)d->m_pIn_buf;
      if (!(*d->m_pPut_buf_func)(d->m_output_buf, n, d->m_pPut_buf_user))
        return (d->m_prev_return_status = TDEFL_STATUS_PUT_BUF_FAILED);
    } else if (pOutput_buf_start == d->m_output_buf) {
      int bytes_to_copy = (int)MZ_MIN(
          (size_t)n, (size_t)(*d->m_pOut_buf_size - d->m_out_buf_ofs));
      memcpy((mz_uint8 *)d->m_pOut_buf + d->m_out_buf_ofs, d->m_output_buf,
             bytes_to_copy);
      d->m_out_buf_ofs += bytes_to_copy;
      if ((n -= bytes_to_copy) != 0) {
        d->m_output_flush_ofs = bytes_to_copy;
        d->m_output_flush_remaining = n;
      }
    } else {
      d->m_out_buf_ofs += n;
    }
  }

  return d->m_output_flush_remaining;
}
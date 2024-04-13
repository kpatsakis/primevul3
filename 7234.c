static mz_bool tdefl_compress_normal(tdefl_compressor *d) {
  const mz_uint8 *pSrc = d->m_pSrc;
  size_t src_buf_left = d->m_src_buf_left;
  tdefl_flush flush = d->m_flush;

  while ((src_buf_left) || ((flush) && (d->m_lookahead_size))) {
    mz_uint len_to_move, cur_match_dist, cur_match_len, cur_pos;
    // Update dictionary and hash chains. Keeps the lookahead size equal to
    // TDEFL_MAX_MATCH_LEN.
    if ((d->m_lookahead_size + d->m_dict_size) >= (TDEFL_MIN_MATCH_LEN - 1)) {
      mz_uint dst_pos = (d->m_lookahead_pos + d->m_lookahead_size) &
                        TDEFL_LZ_DICT_SIZE_MASK,
              ins_pos = d->m_lookahead_pos + d->m_lookahead_size - 2;
      mz_uint hash = (d->m_dict[ins_pos & TDEFL_LZ_DICT_SIZE_MASK]
                      << TDEFL_LZ_HASH_SHIFT) ^
                     d->m_dict[(ins_pos + 1) & TDEFL_LZ_DICT_SIZE_MASK];
      mz_uint num_bytes_to_process = (mz_uint)MZ_MIN(
          src_buf_left, TDEFL_MAX_MATCH_LEN - d->m_lookahead_size);
      const mz_uint8 *pSrc_end = pSrc + num_bytes_to_process;
      src_buf_left -= num_bytes_to_process;
      d->m_lookahead_size += num_bytes_to_process;
      while (pSrc != pSrc_end) {
        mz_uint8 c = *pSrc++;
        d->m_dict[dst_pos] = c;
        if (dst_pos < (TDEFL_MAX_MATCH_LEN - 1))
          d->m_dict[TDEFL_LZ_DICT_SIZE + dst_pos] = c;
        hash = ((hash << TDEFL_LZ_HASH_SHIFT) ^ c) & (TDEFL_LZ_HASH_SIZE - 1);
        d->m_next[ins_pos & TDEFL_LZ_DICT_SIZE_MASK] = d->m_hash[hash];
        d->m_hash[hash] = (mz_uint16)(ins_pos);
        dst_pos = (dst_pos + 1) & TDEFL_LZ_DICT_SIZE_MASK;
        ins_pos++;
      }
    } else {
      while ((src_buf_left) && (d->m_lookahead_size < TDEFL_MAX_MATCH_LEN)) {
        mz_uint8 c = *pSrc++;
        mz_uint dst_pos = (d->m_lookahead_pos + d->m_lookahead_size) &
                          TDEFL_LZ_DICT_SIZE_MASK;
        src_buf_left--;
        d->m_dict[dst_pos] = c;
        if (dst_pos < (TDEFL_MAX_MATCH_LEN - 1))
          d->m_dict[TDEFL_LZ_DICT_SIZE + dst_pos] = c;
        if ((++d->m_lookahead_size + d->m_dict_size) >= TDEFL_MIN_MATCH_LEN) {
          mz_uint ins_pos = d->m_lookahead_pos + (d->m_lookahead_size - 1) - 2;
          mz_uint hash = ((d->m_dict[ins_pos & TDEFL_LZ_DICT_SIZE_MASK]
                           << (TDEFL_LZ_HASH_SHIFT * 2)) ^
                          (d->m_dict[(ins_pos + 1) & TDEFL_LZ_DICT_SIZE_MASK]
                           << TDEFL_LZ_HASH_SHIFT) ^
                          c) &
                         (TDEFL_LZ_HASH_SIZE - 1);
          d->m_next[ins_pos & TDEFL_LZ_DICT_SIZE_MASK] = d->m_hash[hash];
          d->m_hash[hash] = (mz_uint16)(ins_pos);
        }
      }
    }
    d->m_dict_size =
        MZ_MIN(TDEFL_LZ_DICT_SIZE - d->m_lookahead_size, d->m_dict_size);
    if ((!flush) && (d->m_lookahead_size < TDEFL_MAX_MATCH_LEN)) break;

    // Simple lazy/greedy parsing state machine.
    len_to_move = 1;
    cur_match_dist = 0;
    cur_match_len =
        d->m_saved_match_len ? d->m_saved_match_len : (TDEFL_MIN_MATCH_LEN - 1);
    cur_pos = d->m_lookahead_pos & TDEFL_LZ_DICT_SIZE_MASK;
    if (d->m_flags & (TDEFL_RLE_MATCHES | TDEFL_FORCE_ALL_RAW_BLOCKS)) {
      if ((d->m_dict_size) && (!(d->m_flags & TDEFL_FORCE_ALL_RAW_BLOCKS))) {
        mz_uint8 c = d->m_dict[(cur_pos - 1) & TDEFL_LZ_DICT_SIZE_MASK];
        cur_match_len = 0;
        while (cur_match_len < d->m_lookahead_size) {
          if (d->m_dict[cur_pos + cur_match_len] != c) break;
          cur_match_len++;
        }
        if (cur_match_len < TDEFL_MIN_MATCH_LEN)
          cur_match_len = 0;
        else
          cur_match_dist = 1;
      }
    } else {
      tdefl_find_match(d, d->m_lookahead_pos, d->m_dict_size,
                       d->m_lookahead_size, &cur_match_dist, &cur_match_len);
    }
    if (((cur_match_len == TDEFL_MIN_MATCH_LEN) &&
         (cur_match_dist >= 8U * 1024U)) ||
        (cur_pos == cur_match_dist) ||
        ((d->m_flags & TDEFL_FILTER_MATCHES) && (cur_match_len <= 5))) {
      cur_match_dist = cur_match_len = 0;
    }
    if (d->m_saved_match_len) {
      if (cur_match_len > d->m_saved_match_len) {
        tdefl_record_literal(d, (mz_uint8)d->m_saved_lit);
        if (cur_match_len >= 128) {
          tdefl_record_match(d, cur_match_len, cur_match_dist);
          d->m_saved_match_len = 0;
          len_to_move = cur_match_len;
        } else {
          d->m_saved_lit = d->m_dict[cur_pos];
          d->m_saved_match_dist = cur_match_dist;
          d->m_saved_match_len = cur_match_len;
        }
      } else {
        tdefl_record_match(d, d->m_saved_match_len, d->m_saved_match_dist);
        len_to_move = d->m_saved_match_len - 1;
        d->m_saved_match_len = 0;
      }
    } else if (!cur_match_dist)
      tdefl_record_literal(d,
                           d->m_dict[MZ_MIN(cur_pos, sizeof(d->m_dict) - 1)]);
    else if ((d->m_greedy_parsing) || (d->m_flags & TDEFL_RLE_MATCHES) ||
             (cur_match_len >= 128)) {
      tdefl_record_match(d, cur_match_len, cur_match_dist);
      len_to_move = cur_match_len;
    } else {
      d->m_saved_lit = d->m_dict[MZ_MIN(cur_pos, sizeof(d->m_dict) - 1)];
      d->m_saved_match_dist = cur_match_dist;
      d->m_saved_match_len = cur_match_len;
    }
    // Move the lookahead forward by len_to_move bytes.
    d->m_lookahead_pos += len_to_move;
    MZ_ASSERT(d->m_lookahead_size >= len_to_move);
    d->m_lookahead_size -= len_to_move;
    d->m_dict_size =
        MZ_MIN(d->m_dict_size + len_to_move, (mz_uint)TDEFL_LZ_DICT_SIZE);
    // Check if it's time to flush the current LZ codes to the internal output
    // buffer.
    if ((d->m_pLZ_code_buf > &d->m_lz_code_buf[TDEFL_LZ_CODE_BUF_SIZE - 8]) ||
        ((d->m_total_lz_bytes > 31 * 1024) &&
         (((((mz_uint)(d->m_pLZ_code_buf - d->m_lz_code_buf) * 115) >> 7) >=
           d->m_total_lz_bytes) ||
          (d->m_flags & TDEFL_FORCE_ALL_RAW_BLOCKS)))) {
      int n;
      d->m_pSrc = pSrc;
      d->m_src_buf_left = src_buf_left;
      if ((n = tdefl_flush_block(d, 0)) != 0)
        return (n < 0) ? MZ_FALSE : MZ_TRUE;
    }
  }

  d->m_pSrc = pSrc;
  d->m_src_buf_left = src_buf_left;
  return MZ_TRUE;
}
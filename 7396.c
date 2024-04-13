static mz_bool tdefl_output_buffer_putter(const void *pBuf, int len,
                                          void *pUser) {
  tdefl_output_buffer *p = (tdefl_output_buffer *)pUser;
  size_t new_size = p->m_size + len;
  if (new_size > p->m_capacity) {
    size_t new_capacity = p->m_capacity;
    mz_uint8 *pNew_buf;
    if (!p->m_expandable) return MZ_FALSE;
    do {
      new_capacity = MZ_MAX(128U, new_capacity << 1U);
    } while (new_size > new_capacity);
    pNew_buf = (mz_uint8 *)MZ_REALLOC(p->m_pBuf, new_capacity);
    if (!pNew_buf) return MZ_FALSE;
    p->m_pBuf = pNew_buf;
    p->m_capacity = new_capacity;
  }
  memcpy((mz_uint8 *)p->m_pBuf + p->m_size, pBuf, len);
  p->m_size = new_size;
  return MZ_TRUE;
}
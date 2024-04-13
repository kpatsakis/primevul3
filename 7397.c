static mz_bool mz_zip_writer_add_put_buf_callback(const void *pBuf, int len,
                                                  void *pUser) {
  mz_zip_writer_add_state *pState = (mz_zip_writer_add_state *)pUser;
  if ((int)pState->m_pZip->m_pWrite(pState->m_pZip->m_pIO_opaque,
                                    pState->m_cur_archive_file_ofs, pBuf,
                                    len) != len)
    return MZ_FALSE;
  pState->m_cur_archive_file_ofs += len;
  pState->m_comp_size += len;
  return MZ_TRUE;
}
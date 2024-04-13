static mz_bool mz_zip_writer_write_zeros(mz_zip_archive *pZip,
                                         mz_uint64 cur_file_ofs, mz_uint32 n) {
  char buf[4096];
  memset(buf, 0, MZ_MIN(sizeof(buf), n));
  while (n) {
    mz_uint32 s = MZ_MIN(sizeof(buf), n);
    if (pZip->m_pWrite(pZip->m_pIO_opaque, cur_file_ofs, buf, s) != s)
      return MZ_FALSE;
    cur_file_ofs += s;
    n -= s;
  }
  return MZ_TRUE;
}
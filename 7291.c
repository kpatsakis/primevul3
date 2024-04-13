const char *mz_error(int err) {
  static struct {
    int m_err;
    const char *m_pDesc;
  } s_error_descs[] = {{MZ_OK, ""},
                       {MZ_STREAM_END, "stream end"},
                       {MZ_NEED_DICT, "need dictionary"},
                       {MZ_ERRNO, "file error"},
                       {MZ_STREAM_ERROR, "stream error"},
                       {MZ_DATA_ERROR, "data error"},
                       {MZ_MEM_ERROR, "out of memory"},
                       {MZ_BUF_ERROR, "buf error"},
                       {MZ_VERSION_ERROR, "version error"},
                       {MZ_PARAM_ERROR, "parameter error"}};
  mz_uint i;
  for (i = 0; i < sizeof(s_error_descs) / sizeof(s_error_descs[0]); ++i)
    if (s_error_descs[i].m_err == err) return s_error_descs[i].m_pDesc;
  return NULL;
}
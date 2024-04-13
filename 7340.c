static FILE *mz_freopen(const char *pPath, const char *pMode, FILE *pStream) {
  FILE *pFile = NULL;
  if (freopen_s(&pFile, pPath, pMode, pStream)) return NULL;
  return pFile;
}
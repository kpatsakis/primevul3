static FILE *mz_fopen(const char *pFilename, const char *pMode) {
  FILE *pFile = NULL;
  fopen_s(&pFile, pFilename, pMode);
  return pFile;
}
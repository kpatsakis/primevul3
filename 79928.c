bool GetTempDir(FilePath* path) {
  const char* tmp = getenv("TMPDIR");
  if (tmp)
    *path = FilePath(tmp);
  else
#if defined(OS_ANDROID)
    return PathService::Get(base::DIR_CACHE, path);
#else
    *path = FilePath("/tmp");
#endif
  return true;
}

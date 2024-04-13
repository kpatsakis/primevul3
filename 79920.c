bool GetCurrentDirectory(FilePath* dir) {
  base::ThreadRestrictions::AssertIOAllowed();

  char system_buffer[PATH_MAX] = "";
  if (!getcwd(system_buffer, sizeof(system_buffer))) {
    NOTREACHED();
    return false;
  }
  *dir = FilePath(system_buffer);
  return true;
}

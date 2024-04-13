FilePath GetHomeDir() {
#if defined(OS_CHROMEOS)
  if (base::chromeos::IsRunningOnChromeOS())
    return FilePath("/home/chronos/user");
#endif

  const char* home_dir = getenv("HOME");
  if (home_dir && home_dir[0])
    return FilePath(home_dir);

#if defined(OS_ANDROID)
  DLOG(WARNING) << "OS_ANDROID: Home directory lookup not yet implemented.";
#else
  base::ThreadRestrictions::AssertIOAllowed();

  home_dir = g_get_home_dir();
  if (home_dir && home_dir[0])
    return FilePath(home_dir);
#endif

  FilePath rv;
  if (file_util::GetTempDir(&rv))
    return rv;

  return FilePath("/tmp");
}

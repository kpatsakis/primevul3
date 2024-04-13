get_user_challenge_file(YK_KEY *yk, const char *chalresp_path, const struct passwd *user, char **fn, FILE *debug_file)
{
  /* Getting file from user home directory, i.e. ~/.yubico/challenge, or
   * from a system wide directory.
   */

  /* The challenge to use is located in a file in the user's home directory,
   * which therefor can't be encrypted. If an encrypted home directory is used,
   * the option chalresp_path can be used to point to a system-wide directory.
   */

  const char *filename = NULL; /* not including directory */
  char *ptr = NULL;
  unsigned int serial = 0;
  int ret;

  if (! yk_get_serial(yk, 0, 0, &serial)) {
    D (debug_file, "Failed to read serial number (serial-api-visible disabled?).");
    if (! chalresp_path)
      filename = "challenge";
    else
      filename = user->pw_name;
  } else {
    /* We have serial number */
    /* 0xffffffff == 4294967295 == 10 digits */
    size_t len = strlen(chalresp_path == NULL ? "challenge" : user->pw_name) + 1 + 10 + 1;
    if ((ptr = malloc(len)) != NULL) {
      int res = snprintf(ptr, len, "%s-%u", chalresp_path == NULL ? "challenge" : user->pw_name, serial);
      filename = ptr;
      if (res < 0 || (unsigned long)res > len) {
	/* Not enough space, strangely enough. */
	free(ptr);
	filename = NULL;
      }
    }
  }

  if (filename == NULL)
    return 0;

  ret = get_user_cfgfile_path (chalresp_path, filename, user, fn);
  if(ptr) {
    free(ptr);
  }
  return ret;
}

get_user_cfgfile_path(const char *common_path, const char *filename, const struct passwd *user, char **fn)
{
  /* Getting file from user home directory, e.g. ~/.yubico/challenge, or
   * from a system wide directory.
   *
   * Format is hex(challenge):hex(response):slot num
   */
  char *userfile;
  size_t len;

  if (common_path != NULL) {
    len = strlen(common_path) + 1 + strlen(filename) + 1;
    if ((userfile = malloc(len)) == NULL) {
      return 0;
    }
    snprintf(userfile, len, "%s/%s", common_path, filename);
    *fn = userfile;
    return 1;
  }

  /* No common path provided. Construct path to user's ~/.yubico/filename */

  len = strlen(user->pw_dir) + 9 + strlen(filename) + 1;
  if ((userfile = malloc(len)) == NULL) {
    return 0;
  }
  snprintf(userfile, len, "%s/.yubico/%s", user->pw_dir, filename);
  *fn = userfile;
  return 1;
}

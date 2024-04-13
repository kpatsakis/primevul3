check_user_challenge_file(const char *chalresp_path, const struct passwd *user, FILE *debug_file)
{
  /*
   * This function will look for users challenge files.
   *
   * Returns one of AUTH_FOUND, AUTH_NOT_FOUND, AUTH_ERROR
   */
  size_t len;
  int r;
  int ret = AUTH_NOT_FOUND;
  char *userfile = NULL;
  char *userfile_pattern = NULL;
  glob_t userfile_glob;
  const char *filename = NULL;

  if (! chalresp_path) {
    filename = "challenge";
  } else {
    filename = user->pw_name;
  }

  /* check for userfile challenge files */
  r = get_user_cfgfile_path(chalresp_path, filename, user, &userfile);
  if (!r) {
    D (debug_file, "Failed to get user cfgfile path");
    ret = AUTH_ERROR;
    goto out;
  }

  if (!access(userfile, F_OK)) {
    ret = AUTH_FOUND;
    goto out;
  }

  /* check for userfile-* challenge files */
  len = strlen(userfile) + 2 + 1;
  if ((userfile_pattern = malloc(len)) == NULL) {
    D (debug_file, "Failed to allocate memory for userfile pattern: %s", strerror(errno));
    ret = AUTH_ERROR;
    goto out;
  }
  snprintf(userfile_pattern, len, "%s-*", userfile);

  r = glob(userfile_pattern, 0, NULL, &userfile_glob);
  globfree(&userfile_glob);
  switch (r) {
    case GLOB_NOMATCH:
      /* No matches found, so continue */
      break;
    case 0:
      ret = AUTH_FOUND;
      goto out;
    default:
      D (debug_file, "Error while checking for %s challenge files: %s", userfile_pattern, strerror(errno));
      ret = AUTH_ERROR;
      goto out;
  }

out:
  free(userfile_pattern);
  free(userfile);
  return ret;
}

static void cache_expand(char *dst, size_t dstlen, struct Account *acct, char *src)
{
  char *c = NULL;
  char file[PATH_MAX];

  /* server subdirectory */
  if (acct)
  {
    struct Url url;

    mutt_account_tourl(acct, &url);
    url.path = src;
    url_tostring(&url, file, sizeof(file), U_PATH);
  }
  else
    mutt_str_strfcpy(file, src ? src : "", sizeof(file));

  snprintf(dst, dstlen, "%s/%s", NewsCacheDir, file);

  /* remove trailing slash */
  c = dst + strlen(dst) - 1;
  if (*c == '/')
    *c = '\0';
  mutt_expand_path(dst, dstlen);
  mutt_encode_path(dst, dstlen, dst);
}

const char *nntp_format_str(char *buf, size_t buflen, size_t col, int cols, char op,
                            const char *src, const char *prec, const char *if_str,
                            const char *else_str, unsigned long data, enum FormatFlag flags)
{
  struct NntpServer *nserv = (struct NntpServer *) data;
  struct Account *acct = &nserv->conn->account;
  struct Url url;
  char fn[SHORT_STRING], fmt[SHORT_STRING], *p = NULL;

  switch (op)
  {
    case 'a':
      mutt_account_tourl(acct, &url);
      url_tostring(&url, fn, sizeof(fn), U_PATH);
      p = strchr(fn, '/');
      if (p)
        *p = '\0';
      snprintf(fmt, sizeof(fmt), "%%%ss", prec);
      snprintf(buf, buflen, fmt, fn);
      break;
    case 'p':
      snprintf(fmt, sizeof(fmt), "%%%su", prec);
      snprintf(buf, buflen, fmt, acct->port);
      break;
    case 'P':
      *buf = '\0';
      if (acct->flags & MUTT_ACCT_PORT)
      {
        snprintf(fmt, sizeof(fmt), "%%%su", prec);
        snprintf(buf, buflen, fmt, acct->port);
      }
      break;
    case 's':
      strncpy(fn, acct->host, sizeof(fn) - 1);
      mutt_str_strlower(fn);
      snprintf(fmt, sizeof(fmt), "%%%ss", prec);
      snprintf(buf, buflen, fmt, fn);
      break;
    case 'S':
      mutt_account_tourl(acct, &url);
      url_tostring(&url, fn, sizeof(fn), U_PATH);
      p = strchr(fn, ':');
      if (p)
        *p = '\0';
      snprintf(fmt, sizeof(fmt), "%%%ss", prec);
      snprintf(buf, buflen, fmt, fn);
      break;
    case 'u':
      snprintf(fmt, sizeof(fmt), "%%%ss", prec);
      snprintf(buf, buflen, fmt, acct->user);
      break;
  }
  return src;
}

static int pop_open_mailbox(struct Context *ctx)
{
  char buf[PATH_MAX];
  struct Connection *conn = NULL;
  struct Account acct;
  struct PopData *pop_data = NULL;
  struct Url url;

  if (pop_parse_path(ctx->path, &acct))
  {
    mutt_error(_("%s is an invalid POP path"), ctx->path);
    return -1;
  }

  mutt_account_tourl(&acct, &url);
  url.path = NULL;
  url_tostring(&url, buf, sizeof(buf), 0);
  conn = mutt_conn_find(NULL, &acct);
  if (!conn)
    return -1;

  FREE(&ctx->path);
  FREE(&ctx->realpath);
  ctx->path = mutt_str_strdup(buf);
  ctx->realpath = mutt_str_strdup(ctx->path);

  pop_data = mutt_mem_calloc(1, sizeof(struct PopData));
  pop_data->conn = conn;
  ctx->data = pop_data;

  if (pop_open_connection(pop_data) < 0)
    return -1;

  conn->data = pop_data;
  pop_data->bcache = mutt_bcache_open(&acct, NULL);

  /* init (hard-coded) ACL rights */
  memset(ctx->rights, 0, sizeof(ctx->rights));
  mutt_bit_set(ctx->rights, MUTT_ACL_SEEN);
  mutt_bit_set(ctx->rights, MUTT_ACL_DELETE);
#ifdef USE_HCACHE
  /* flags are managed using header cache, so it only makes sense to
   * enable them in that case */
  mutt_bit_set(ctx->rights, MUTT_ACL_WRITE);
#endif

  while (true)
  {
    if (pop_reconnect(ctx) < 0)
      return -1;

    ctx->size = pop_data->size;

    mutt_message(_("Fetching list of messages..."));

    const int ret = pop_fetch_headers(ctx);

    if (ret >= 0)
      return 0;

    if (ret < -1)
    {
      mutt_sleep(2);
      return -1;
    }
  }
}

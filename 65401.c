static header_cache_t *pop_hcache_open(struct PopData *pop_data, const char *path)
{
  struct Url url;
  char p[LONG_STRING];

  if (!pop_data || !pop_data->conn)
    return mutt_hcache_open(HeaderCache, path, NULL);

  mutt_account_tourl(&pop_data->conn->account, &url);
  url.path = HC_FNAME;
  url_tostring(&url, p, sizeof(p), U_PATH);
  return mutt_hcache_open(HeaderCache, p, pop_hcache_namer);
}

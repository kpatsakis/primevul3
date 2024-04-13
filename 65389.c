void nntp_delete_group_cache(struct NntpData *nntp_data)
{
  if (!nntp_data || !nntp_data->nserv || !nntp_data->nserv->cacheable)
    return;

#ifdef USE_HCACHE
  char file[PATH_MAX];
  nntp_hcache_namer(nntp_data->group, file, sizeof(file));
  cache_expand(file, sizeof(file), &nntp_data->nserv->conn->account, file);
  unlink(file);
  nntp_data->last_cached = 0;
  mutt_debug(2, "%s\n", file);
#endif

  if (!nntp_data->bcache)
  {
    nntp_data->bcache =
        mutt_bcache_open(&nntp_data->nserv->conn->account, nntp_data->group);
  }
  if (nntp_data->bcache)
  {
    mutt_debug(2, "%s/*\n", nntp_data->group);
    mutt_bcache_list(nntp_data->bcache, nntp_bcache_delete, NULL);
    mutt_bcache_close(&nntp_data->bcache);
  }
}

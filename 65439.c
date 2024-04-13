static int msg_cache_clean_cb(const char *id, struct BodyCache *bcache, void *data)
{
  unsigned int uv, uid;
  struct ImapData *idata = data;

  if (sscanf(id, "%u-%u", &uv, &uid) != 2)
    return 0;

  /* bad UID */
  if (uv != idata->uid_validity || !mutt_hash_int_find(idata->uid_hash, uid))
    mutt_bcache_del(bcache, id);

  return 0;
}

static FILE *msg_cache_get(struct ImapData *idata, struct Header *h)
{
  if (!idata || !h)
    return NULL;

  idata->bcache = msg_cache_open(idata);
  char id[64];
  snprintf(id, sizeof(id), "%u-%u", idata->uid_validity, HEADER_DATA(h)->uid);
  return mutt_bcache_get(idata->bcache, id);
}

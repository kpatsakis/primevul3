int imap_cache_del(struct ImapData *idata, struct Header *h)
{
  if (!idata || !h)
    return -1;

  idata->bcache = msg_cache_open(idata);
  char id[64];
  snprintf(id, sizeof(id), "%u-%u", idata->uid_validity, HEADER_DATA(h)->uid);
  return mutt_bcache_del(idata->bcache, id);
}

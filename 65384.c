static int nntp_bcache_delete(const char *id, struct BodyCache *bcache, void *data)
{
  struct NntpData *nntp_data = data;
  anum_t anum;
  char c;

  if (!nntp_data || sscanf(id, ANUM "%c", &anum, &c) != 1 ||
      anum < nntp_data->first_message || anum > nntp_data->last_message)
  {
    if (nntp_data)
      mutt_debug(2, "mutt_bcache_del %s\n", id);
    mutt_bcache_del(bcache, id);
  }
  return 0;
}

void nntp_bcache_update(struct NntpData *nntp_data)
{
  mutt_bcache_list(nntp_data->bcache, nntp_bcache_delete, nntp_data);
}

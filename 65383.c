void nntp_article_status(struct Context *ctx, struct Header *hdr, char *group, anum_t anum)
{
  struct NntpData *nntp_data = ctx->data;

  if (group)
    nntp_data = mutt_hash_find(nntp_data->nserv->groups_hash, group);

  if (!nntp_data)
    return;

  for (unsigned int i = 0; i < nntp_data->newsrc_len; i++)
  {
    if ((anum >= nntp_data->newsrc_ent[i].first) &&
        (anum <= nntp_data->newsrc_ent[i].last))
    {
      /* can't use mutt_set_flag() because mx_update_context()
         didn't called yet */
      hdr->read = true;
      return;
    }
  }

  /* article was not cached yet, it's new */
  if (anum > nntp_data->last_cached)
    return;

  /* article isn't read but cached, it's old */
  if (MarkOld)
    hdr->old = true;
}

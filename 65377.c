struct NntpData *mutt_newsgroup_catchup(struct NntpServer *nserv, char *group)
{
  struct NntpData *nntp_data = NULL;

  if (!nserv || !nserv->groups_hash || !group || !*group)
    return NULL;

  nntp_data = mutt_hash_find(nserv->groups_hash, group);
  if (!nntp_data)
    return NULL;

  if (nntp_data->newsrc_ent)
  {
    mutt_mem_realloc(&nntp_data->newsrc_ent, sizeof(struct NewsrcEntry));
    nntp_data->newsrc_len = 1;
    nntp_data->newsrc_ent[0].first = 1;
    nntp_data->newsrc_ent[0].last = nntp_data->last_message;
  }
  nntp_data->unread = 0;
  if (Context && Context->data == nntp_data)
  {
    for (unsigned int i = 0; i < Context->msgcount; i++)
      mutt_set_flag(Context, Context->hdrs[i], MUTT_READ, 1);
  }
  return nntp_data;
}

struct NntpData *mutt_newsgroup_uncatchup(struct NntpServer *nserv, char *group)
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
    nntp_data->newsrc_ent[0].last = nntp_data->first_message - 1;
  }
  if (Context && Context->data == nntp_data)
  {
    nntp_data->unread = Context->msgcount;
    for (unsigned int i = 0; i < Context->msgcount; i++)
      mutt_set_flag(Context, Context->hdrs[i], MUTT_READ, 0);
  }
  else
  {
    nntp_data->unread = nntp_data->last_message;
    if (nntp_data->newsrc_ent)
      nntp_data->unread -= nntp_data->newsrc_ent[0].last;
  }
  return nntp_data;
}

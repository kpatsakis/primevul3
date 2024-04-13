struct NntpData *mutt_newsgroup_unsubscribe(struct NntpServer *nserv, char *group)
{
  struct NntpData *nntp_data = NULL;

  if (!nserv || !nserv->groups_hash || !group || !*group)
    return NULL;

  nntp_data = mutt_hash_find(nserv->groups_hash, group);
  if (!nntp_data)
    return NULL;

  nntp_data->subscribed = false;
  if (!SaveUnsubscribed)
  {
    nntp_data->newsrc_len = 0;
    FREE(&nntp_data->newsrc_ent);
  }
  return nntp_data;
}

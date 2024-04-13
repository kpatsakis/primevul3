static struct NntpData *nntp_data_find(struct NntpServer *nserv, const char *group)
{
  struct NntpData *nntp_data = mutt_hash_find(nserv->groups_hash, group);
  if (nntp_data)
    return nntp_data;

  size_t len = strlen(group) + 1;
  /* create NntpData structure and add it to hash */
  nntp_data = mutt_mem_calloc(1, sizeof(struct NntpData) + len);
  nntp_data->group = (char *) nntp_data + sizeof(struct NntpData);
  mutt_str_strfcpy(nntp_data->group, group, len);
  nntp_data->nserv = nserv;
  nntp_data->deleted = true;
  mutt_hash_insert(nserv->groups_hash, nntp_data->group, nntp_data);

  /* add NntpData to list */
  if (nserv->groups_num >= nserv->groups_max)
  {
    nserv->groups_max *= 2;
    mutt_mem_realloc(&nserv->groups_list, nserv->groups_max * sizeof(nntp_data));
  }
  nserv->groups_list[nserv->groups_num++] = nntp_data;

  return nntp_data;
}

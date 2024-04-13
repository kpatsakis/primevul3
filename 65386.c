void nntp_buffy(char *buf, size_t len)
{
  for (unsigned int i = 0; i < CurrentNewsSrv->groups_num; i++)
  {
    struct NntpData *nntp_data = CurrentNewsSrv->groups_list[i];

    if (!nntp_data || !nntp_data->subscribed || !nntp_data->unread)
      continue;

    if (Context && Context->magic == MUTT_NNTP &&
        (mutt_str_strcmp(nntp_data->group, ((struct NntpData *) Context->data)->group) == 0))
    {
      unsigned int unread = 0;

      for (unsigned int j = 0; j < Context->msgcount; j++)
        if (!Context->hdrs[j]->read && !Context->hdrs[j]->deleted)
          unread++;
      if (!unread)
        continue;
    }
    mutt_str_strfcpy(buf, nntp_data->group, len);
    break;
  }
}

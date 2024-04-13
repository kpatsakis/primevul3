int nntp_newsrc_update(struct NntpServer *nserv)
{
  char *buf = NULL;
  size_t buflen, off;
  int rc = -1;

  if (!nserv)
    return -1;

  buflen = 10 * LONG_STRING;
  buf = mutt_mem_calloc(1, buflen);
  off = 0;

  /* we will generate full newsrc here */
  for (unsigned int i = 0; i < nserv->groups_num; i++)
  {
    struct NntpData *nntp_data = nserv->groups_list[i];

    if (!nntp_data || !nntp_data->newsrc_ent)
      continue;

    /* write newsgroup name */
    if (off + strlen(nntp_data->group) + 3 > buflen)
    {
      buflen *= 2;
      mutt_mem_realloc(&buf, buflen);
    }
    snprintf(buf + off, buflen - off, "%s%c ", nntp_data->group,
             nntp_data->subscribed ? ':' : '!');
    off += strlen(buf + off);

    /* write entries */
    for (unsigned int j = 0; j < nntp_data->newsrc_len; j++)
    {
      if (off + LONG_STRING > buflen)
      {
        buflen *= 2;
        mutt_mem_realloc(&buf, buflen);
      }
      if (j)
        buf[off++] = ',';
      if (nntp_data->newsrc_ent[j].first == nntp_data->newsrc_ent[j].last)
        snprintf(buf + off, buflen - off, "%u", nntp_data->newsrc_ent[j].first);
      else if (nntp_data->newsrc_ent[j].first < nntp_data->newsrc_ent[j].last)
      {
        snprintf(buf + off, buflen - off, "%u-%u",
                 nntp_data->newsrc_ent[j].first, nntp_data->newsrc_ent[j].last);
      }
      off += strlen(buf + off);
    }
    buf[off++] = '\n';
  }
  buf[off] = '\0';

  /* newrc being fully rewritten */
  mutt_debug(1, "Updating %s\n", nserv->newsrc_file);
  if (nserv->newsrc_file && update_file(nserv->newsrc_file, buf) == 0)
  {
    struct stat sb;

    rc = stat(nserv->newsrc_file, &sb);
    if (rc == 0)
    {
      nserv->size = sb.st_size;
      nserv->mtime = sb.st_mtime;
    }
    else
    {
      mutt_perror(nserv->newsrc_file);
    }
  }
  FREE(&buf);
  return rc;
}

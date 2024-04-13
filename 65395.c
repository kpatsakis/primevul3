int nntp_newsrc_parse(struct NntpServer *nserv)
{
  char *line = NULL;
  struct stat sb;

  if (nserv->newsrc_fp)
  {
    /* if we already have a handle, close it and reopen */
    mutt_file_fclose(&nserv->newsrc_fp);
  }
  else
  {
    /* if file doesn't exist, create it */
    nserv->newsrc_fp = mutt_file_fopen(nserv->newsrc_file, "a");
    mutt_file_fclose(&nserv->newsrc_fp);
  }

  /* open .newsrc */
  nserv->newsrc_fp = mutt_file_fopen(nserv->newsrc_file, "r");
  if (!nserv->newsrc_fp)
  {
    mutt_perror(nserv->newsrc_file);
    return -1;
  }

  /* lock it */
  mutt_debug(1, "Locking %s\n", nserv->newsrc_file);
  if (mutt_file_lock(fileno(nserv->newsrc_fp), 0, 1))
  {
    mutt_file_fclose(&nserv->newsrc_fp);
    return -1;
  }

  if (stat(nserv->newsrc_file, &sb))
  {
    mutt_perror(nserv->newsrc_file);
    nntp_newsrc_close(nserv);
    return -1;
  }

  if (nserv->size == sb.st_size && nserv->mtime == sb.st_mtime)
    return 0;

  nserv->size = sb.st_size;
  nserv->mtime = sb.st_mtime;
  nserv->newsrc_modified = true;
  mutt_debug(1, "Parsing %s\n", nserv->newsrc_file);

  /* .newsrc has been externally modified or hasn't been loaded yet */
  for (unsigned int i = 0; i < nserv->groups_num; i++)
  {
    struct NntpData *nntp_data = nserv->groups_list[i];

    if (!nntp_data)
      continue;

    nntp_data->subscribed = false;
    nntp_data->newsrc_len = 0;
    FREE(&nntp_data->newsrc_ent);
  }

  line = mutt_mem_malloc(sb.st_size + 1);
  while (sb.st_size && fgets(line, sb.st_size + 1, nserv->newsrc_fp))
  {
    char *b = NULL, *h = NULL;
    unsigned int j = 1;
    bool subs = false;

    /* find end of newsgroup name */
    char *p = strpbrk(line, ":!");
    if (!p)
      continue;

    /* ":" - subscribed, "!" - unsubscribed */
    if (*p == ':')
      subs = true;
    *p++ = '\0';

    /* get newsgroup data */
    struct NntpData *nntp_data = nntp_data_find(nserv, line);
    FREE(&nntp_data->newsrc_ent);

    /* count number of entries */
    b = p;
    while (*b)
      if (*b++ == ',')
        j++;
    nntp_data->newsrc_ent = mutt_mem_calloc(j, sizeof(struct NewsrcEntry));
    nntp_data->subscribed = subs;

    /* parse entries */
    j = 0;
    while (p)
    {
      b = p;

      /* find end of entry */
      p = strchr(p, ',');
      if (p)
        *p++ = '\0';

      /* first-last or single number */
      h = strchr(b, '-');
      if (h)
        *h++ = '\0';
      else
        h = b;

      if (sscanf(b, ANUM, &nntp_data->newsrc_ent[j].first) == 1 &&
          sscanf(h, ANUM, &nntp_data->newsrc_ent[j].last) == 1)
      {
        j++;
      }
    }
    if (j == 0)
    {
      nntp_data->newsrc_ent[j].first = 1;
      nntp_data->newsrc_ent[j].last = 0;
      j++;
    }
    if (nntp_data->last_message == 0)
      nntp_data->last_message = nntp_data->newsrc_ent[j - 1].last;
    nntp_data->newsrc_len = j;
    mutt_mem_realloc(&nntp_data->newsrc_ent, j * sizeof(struct NewsrcEntry));
    nntp_group_unread_stat(nntp_data);
    mutt_debug(2, "%s\n", nntp_data->group);
  }
  FREE(&line);
  return 1;
}

int nntp_active_save_cache(struct NntpServer *nserv)
{
  char file[PATH_MAX];
  char *buf = NULL;
  size_t buflen, off;
  int rc;

  if (!nserv->cacheable)
    return 0;

  buflen = 10 * LONG_STRING;
  buf = mutt_mem_calloc(1, buflen);
  snprintf(buf, buflen, "%lu\n", (unsigned long) nserv->newgroups_time);
  off = strlen(buf);

  for (unsigned int i = 0; i < nserv->groups_num; i++)
  {
    struct NntpData *nntp_data = nserv->groups_list[i];

    if (!nntp_data || nntp_data->deleted)
      continue;

    if (off + strlen(nntp_data->group) + (nntp_data->desc ? strlen(nntp_data->desc) : 0) + 50 > buflen)
    {
      buflen *= 2;
      mutt_mem_realloc(&buf, buflen);
    }
    snprintf(buf + off, buflen - off, "%s %u %u %c%s%s\n", nntp_data->group,
             nntp_data->last_message, nntp_data->first_message,
             nntp_data->allowed ? 'y' : 'n', nntp_data->desc ? " " : "",
             nntp_data->desc ? nntp_data->desc : "");
    off += strlen(buf + off);
  }

  cache_expand(file, sizeof(file), &nserv->conn->account, ".active");
  mutt_debug(1, "Updating %s\n", file);
  rc = update_file(file, buf);
  FREE(&buf);
  return rc;
}

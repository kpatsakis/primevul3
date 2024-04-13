static int active_get_cache(struct NntpServer *nserv)
{
  char buf[HUGE_STRING];
  char file[PATH_MAX];
  time_t t;

  cache_expand(file, sizeof(file), &nserv->conn->account, ".active");
  mutt_debug(1, "Parsing %s\n", file);
  FILE *fp = mutt_file_fopen(file, "r");
  if (!fp)
    return -1;

  if (fgets(buf, sizeof(buf), fp) == NULL || sscanf(buf, "%ld%s", &t, file) != 1 || t == 0)
  {
    mutt_file_fclose(&fp);
    return -1;
  }
  nserv->newgroups_time = t;

  mutt_message(_("Loading list of groups from cache..."));
  while (fgets(buf, sizeof(buf), fp))
    nntp_add_group(buf, nserv);
  nntp_add_group(NULL, NULL);
  mutt_file_fclose(&fp);
  mutt_clear_error();
  return 0;
}

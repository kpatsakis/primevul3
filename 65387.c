void nntp_clear_cache(struct NntpServer *nserv)
{
  char file[PATH_MAX];
  char *fp = NULL;
  struct dirent *entry = NULL;
  DIR *dp = NULL;

  if (!nserv || !nserv->cacheable)
    return;

  cache_expand(file, sizeof(file), &nserv->conn->account, NULL);
  dp = opendir(file);
  if (dp)
  {
    mutt_str_strncat(file, sizeof(file), "/", 1);
    fp = file + strlen(file);
    while ((entry = readdir(dp)))
    {
      char *group = entry->d_name;
      struct stat sb;
      struct NntpData *nntp_data = NULL;
      struct NntpData nntp_tmp;

      if ((mutt_str_strcmp(group, ".") == 0) || (mutt_str_strcmp(group, "..") == 0))
        continue;
      *fp = '\0';
      mutt_str_strncat(file, sizeof(file), group, strlen(group));
      if (stat(file, &sb))
        continue;

#ifdef USE_HCACHE
      if (S_ISREG(sb.st_mode))
      {
        char *ext = group + strlen(group) - 7;
        if (strlen(group) < 8 || (mutt_str_strcmp(ext, ".hcache") != 0))
          continue;
        *ext = '\0';
      }
      else
#endif
          if (!S_ISDIR(sb.st_mode))
        continue;

      nntp_data = mutt_hash_find(nserv->groups_hash, group);
      if (!nntp_data)
      {
        nntp_data = &nntp_tmp;
        nntp_data->nserv = nserv;
        nntp_data->group = group;
        nntp_data->bcache = NULL;
      }
      else if (nntp_data->newsrc_ent || nntp_data->subscribed || SaveUnsubscribed)
        continue;

      nntp_delete_group_cache(nntp_data);
      if (S_ISDIR(sb.st_mode))
      {
        rmdir(file);
        mutt_debug(2, "%s\n", file);
      }
    }
    closedir(dp);
  }
}

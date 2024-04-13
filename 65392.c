void nntp_hcache_update(struct NntpData *nntp_data, header_cache_t *hc)
{
  char buf[16];
  bool old = false;
  void *hdata = NULL;
  anum_t first = 0, last = 0;

  if (!hc)
    return;

  /* fetch previous values of first and last */
  hdata = mutt_hcache_fetch_raw(hc, "index", 5);
  if (hdata)
  {
    mutt_debug(2, "mutt_hcache_fetch index: %s\n", (char *) hdata);
    if (sscanf(hdata, ANUM " " ANUM, &first, &last) == 2)
    {
      old = true;
      nntp_data->last_cached = last;

      /* clean removed headers from cache */
      for (anum_t current = first; current <= last; current++)
      {
        if (current >= nntp_data->first_message && current <= nntp_data->last_message)
          continue;

        snprintf(buf, sizeof(buf), "%u", current);
        mutt_debug(2, "mutt_hcache_delete %s\n", buf);
        mutt_hcache_delete(hc, buf, strlen(buf));
      }
    }
    mutt_hcache_free(hc, &hdata);
  }

  /* store current values of first and last */
  if (!old || nntp_data->first_message != first || nntp_data->last_message != last)
  {
    snprintf(buf, sizeof(buf), "%u %u", nntp_data->first_message, nntp_data->last_message);
    mutt_debug(2, "mutt_hcache_store index: %s\n", buf);
    mutt_hcache_store_raw(hc, "index", 5, buf, strlen(buf));
  }
}

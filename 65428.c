static void alloc_msn_index(struct ImapData *idata, size_t msn_count)
{
  size_t new_size;

  if (msn_count <= idata->msn_index_size)
    return;

  /* This is a conservative check to protect against a malicious imap
   * server.  Most likely size_t is bigger than an unsigned int, but
   * if msn_count is this big, we have a serious problem. */
  if (msn_count >= (UINT_MAX / sizeof(struct Header *)))
  {
    mutt_error(_("Integer overflow -- can't allocate memory."));
    mutt_exit(1);
  }

  /* Add a little padding, like mx_allloc_memory() */
  new_size = msn_count + 25;

  if (!idata->msn_index)
    idata->msn_index = mutt_mem_calloc(new_size, sizeof(struct Header *));
  else
  {
    mutt_mem_realloc(&idata->msn_index, sizeof(struct Header *) * new_size);
    memset(idata->msn_index + idata->msn_index_size, 0,
           sizeof(struct Header *) * (new_size - idata->msn_index_size));
  }

  idata->msn_index_size = new_size;
}

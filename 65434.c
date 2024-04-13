void imap_free_header_data(struct ImapHeaderData **data)
{
  if (!data || !*data)
    return;

  /* this should be safe even if the list wasn't used */
  FREE(&((*data)->flags_system));
  FREE(&((*data)->flags_remote));
  FREE(data);
}
